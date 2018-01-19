;********************************************************************************************************
;                                               uC/OS-II
;                                         The Real-Time Kernel
;
;                               (c) Copyright 1992-2006, Micrium, Weston, FL
;                                          All Rights Reserved
;
;                                           Generic ARM Port
;
; File      : OS_CPU_A.ASM
; Version   : V2.86
; By        : Jean J. Labrosse
;
; For       : ARMv7M Cortex-M3
; Mode      : Thumb2
; Toolchain : RealView Development Suite
;             RealView Microcontroller Development Kit (MDK)
;             ARM Developer Suite (ADS)
;             Keil uVision
;********************************************************************************************************

;********************************************************************************************************
;                                           PUBLIC FUNCTIONS
;********************************************************************************************************

    EXTERN  OSRunning                                           ; External references
    EXTERN  OSPrioCur											;��Ӧ�����õ�������   
    EXTERN  OSPrioHighRdy
    EXTERN  OSTCBCur
    EXTERN  OSTCBHighRdy
    EXTERN  OSIntNesting
    EXTERN  OSIntExit
    EXTERN  OSTaskSwHook


    EXPORT  OS_CPU_SR_Save                                      ; Functions declared in this file
    EXPORT  OS_CPU_SR_Restore								    ;����ⲿ���� 
    EXPORT  OSStartHighRdy
    EXPORT  OSCtxSw
    EXPORT  OSIntCtxSw
    EXPORT  OS_CPU_PendSVHandler

;********************************************************************************************************
;                                                EQUATES
;********************************************************************************************************

NVIC_INT_CTRL   EQU     0xE000ED04                              ; Interrupt control state register.      �жϿ��Ƽ�״̬�Ĵ���ICSR�ĵ�ַ
NVIC_SYSPRI14   EQU     0xE000ED22                              ; System priority register (priority 14).PendSV���ȼ��Ĵ����ĵ�ַ
NVIC_PENDSV_PRI EQU           0xFF                              ; PendSV priority value (lowest).		 PendSV�жϵ����ȼ���255����ͣ�ϵͳ���ȼ��Ĵ���
NVIC_PENDSVSET  EQU     0x10000000                              ; Value to trigger PendSV exception.	 ��Ӧ��λ28��һ������PendSV�ж�
																;

;********************************************************************************************************
;                                      CODE GENERATION DIRECTIVES
;********************************************************************************************************

    AREA |.text|, CODE, READONLY, ALIGN=2
    THUMB
    REQUIRE8
    PRESERVE8

;********************************************************************************************************
;                                   CRITICAL SECTION METHOD 3 FUNCTIONS
;
; Description: Disable/Enable interrupts by preserving the state of interrupts.  Generally speaking you
;              would store the state of the interrupt disable flag in the local variable 'cpu_sr' and then
;              disable interrupts.  'cpu_sr' is allocated in all of uC/OS-II's functions that need to
;              disable interrupts.  You would restore the interrupt disable state by copying back 'cpu_sr'
;              into the CPU's status register.
;
; Prototypes :     OS_CPU_SR  OS_CPU_SR_Save(void);
;                  void       OS_CPU_SR_Restore(OS_CPU_SR cpu_sr);
;
;
; Note(s)    : 1) These functions are used in general like this:
;
;                 void Task (void *p_arg)
;                 {
;                 #if OS_CRITICAL_METHOD == 3          /* Allocate storage for CPU status register */
;                     OS_CPU_SR  cpu_sr;
;                 #endif
;
;                          :
;                          :
;                     OS_ENTER_CRITICAL();             /* cpu_sr = OS_CPU_SaveSR();                */
;                          :
;                          :
;                     OS_EXIT_CRITICAL();              /* OS_CPU_RestoreSR(cpu_sr);                */
;                          :
;                          :
;                 }
;********************************************************************************************************

OS_CPU_SR_Save
    MRS     R0, PRIMASK                                         ;��ȡPRIMASK��R0��R0�Ƿ���ֵ Set prio int mask to mask all (except faults)
    CPSID   I												    ;PRIMASK=1�����жϣ�NMI��Ӳfault������Ӧ��
    BX      LR													;R14 ����

OS_CPU_SR_Restore
    MSR     PRIMASK, R0											;��ȡR0��PRIMASK�У�R0Ϊ����
    BX      LR													;����

;********************************************************************************************************
;                                          START MULTITASKING
;                                       void OSStartHighRdy(void)
;
; Note(s) : 1) This function triggers a PendSV exception (essentially, causes a context switch) to cause
;              the first task to start.
;
;           2) OSStartHighRdy() MUST:
;              a) Setup PendSV exception priority to lowest;
;              b) Set initial PSP to 0, to tell context switcher this is first run;
;              c) Set OSRunning to TRUE;
;              d) Trigger PendSV exception;
;              e) Enable interrupts (tasks will run with interrupts enabled).
;    OSStartHighRdy()��OSStart()���ã���������������ȼ����񣬵�Ȼ���������OSStart()ǰ�ѱ�����
;	 #1.PendSV�жϵ����ȼ�Ӧ��Ϊ������ȼ���ԭ����<<ARM Cortex-M3Ȩ��ָ��>>��7.6������˵��
;    #2.PSP����Ϊ0���Ǹ��߾���������л�����OS_CPU_PendSVHandler()�������ǵ�һ�������л��������л���PSP�Ͳ���Ϊ0�ˣ�����ῴ��
;    #3.���жϿ��Ƽ�״̬�Ĵ���ICSR(0xE000ED04)��28λд1���ɲ���PendSV�жϡ����<<ARM Cortex-M3Ȩ��ָ��>>8.4.5 �����쳣�����üĴ�����˵����		
;********************************************************************************************************

OSStartHighRdy													;����PendSV�жϵ����ȼ�
    LDR     R0, =NVIC_SYSPRI14                                  ;Set the PendSV exception priority
    LDR     R1, =NVIC_PENDSV_PRI
    STRB    R1, [R0]											;*(uint8_t *)NVIC_SYSPRI14 = NVIC_PENDSV_PRI

    MOVS    R0, #0                                              ;��ʼ�����̶�ջָ��  Set the PSP to 0 for initial context switch call
    MSR     PSP, R0												;��ʼ��PSPΪ0        ��ʼ���������л�����

    LDR     R0, =OSRunning                                      ; OSRunning = TRUE
    MOVS    R1, #1												;����OSRunning = TRUE
    STRB    R1, [R0]

    LDR     R0, =NVIC_INT_CTRL                                  ; Trigger the PendSV exception (causes context switch)
    LDR     R1, =NVIC_PENDSVSET									;����PendSV�ж�
    STR     R1, [R0]											;*(uint32_t *)NVIC_INT_CTRL = NVIC_PENDSVSET


    CPSIE   I                                                   ; Enable interrupts at processor level�����ж�

OSStartHang
    B       OSStartHang                                         ; Should never get here ��ѭ�� which��1����


;********************************************************************************************************
;                               PERFORM A CONTEXT SWITCH (From task level)
;                                           void OSCtxSw(void)
;
; Note(s) : 1) OSCtxSw() is called when OS wants to perform a task context switch.  This function
;              triggers the PendSV exception which is where the real work is done.
; ��һ���������cpu��ʹ��Ȩ���ͻ����OS_TASK_SW()�꣬��OS_TASK_SW()����OSCtxSw()��OSCtxSw()Ӧ���������л���
; ������CM3�У����������л������ŵ�PendSV���жϴ�������ȥ���ˣ����OSCtxSw()ֻ��򵥵Ĵ���PendSV�жϼ��ɡ�
; OS_TASK_SW()����OS_Sched()���á�
;********************************************************************************************************

OSCtxSw
    LDR     R0, =NVIC_INT_CTRL                                  ; Trigger the PendSV exception (causes context switch)
    LDR     R1, =NVIC_PENDSVSET
    STR     R1, [R0]											;*(uint32_t *)NVIC_INT_CTRL = NVIC_PENDSVSET
    BX      LR													;����

;********************************************************************************************************
;                             PERFORM A CONTEXT SWITCH (From interrupt level)
;                                         void OSIntCtxSw(void)
;
; Notes:    1) OSIntCtxSw() is called by OSIntExit() when it determines a context switch is needed as
;              the result of an interrupt.  This function simply triggers a PendSV exception which will
;              be handled when there are no more interrupts active and interrupts are enabled.
;����������Щͬѧ���������ôOSCtxSw()��OSIntCtxSw()��ȫһ������ʵ�ϣ������������������ǲ�һ���ģ�OSCtxSw()
;����������֮����л���������A��Ϊ�ȴ�ĳ����Դ��������ʱ�л�������B����OSIntCtxSw()�����ж��˳�ʱ�����ж�״
;̬�л�����һ���������ж��л�������ʱ��CPU�Ĵ�����ջ�Ĺ����Ѿ������ˣ������������ڶ����ˣ��ο�����ʦ���3.10�ڣ���
;����ֻ��������CM3��������Ƶ�������������������ֻҪ������PendSV�жϼ��ɣ������л���PendSV�ж�������
;********************************************************************************************************

OSIntCtxSw
    LDR     R0, =NVIC_INT_CTRL                                  ; Trigger the PendSV exception (causes context switch)
    LDR     R1, =NVIC_PENDSVSET
    STR     R1, [R0]
    BX      LR

;********************************************************************************************************
;                                         HANDLE PendSV EXCEPTION
;                                     void OS_CPU_PendSVHandler(void)
;
; Note(s) : 1) PendSV is used to cause a context switch.  This is a recommended method for performing
;              context switches with Cortex-M3.  This is because the Cortex-M3 auto-saves half of the
;              processor context on any exception, and restores same on return from exception.  So only
;              saving of R4-R11 is required and fixing up the stack pointers.  Using the PendSV exception
;              this way means that context saving and restoring is identical whether it is initiated from
;              a thread or occurs due to an interrupt or exception.
;
;           2) Pseudo-code is:
;              a) Get the process SP, if 0 then skip (goto d) the saving part (first context switch);
;              b) Save remaining regs r4-r11 on process stack;
;              c) Save the process SP in its TCB, OSTCBCur->OSTCBStkPtr = SP;
;              d) Call OSTaskSwHook();
;              e) Get current high priority, OSPrioCur = OSPrioHighRdy;
;              f) Get current ready thread TCB, OSTCBCur = OSTCBHighRdy;
;              g) Get new process SP from TCB, SP = OSTCBHighRdy->OSTCBStkPtr;
;              h) Restore R4-R11 from new process stack;
;              i) Perform exception return which will restore remaining context.
;
;           3) On entry into PendSV handler:
;              a) The following have been saved on the process stack (by processor):
;                 xPSR, PC, LR, R12, R0-R3
;              b) Processor mode is switched to Handler mode (from Thread mode)
;              c) Stack is Main stack (switched from Process stack)
;              d) OSTCBCur      points to the OS_TCB of the task to suspend
;                 OSTCBHighRdy  points to the OS_TCB of the task to resume
;
;           4) Since PendSV is set to lowest priority in the system (by OSStartHighRdy() above), we
;              know that it will only be run when no other exception or interrupt is active, and
;              therefore safe to assume that context being switched out was using the process stack (PSP).
;              ǰ���Ѿ�˵�������������л�����PendSV�жϴ����������ģ�����CM3���ж�ʱ����һ��ļĴ����Զ����浽�����ջ�
;              ������PendSV�жϴ�������ֻ�豣��R4-R11�����ڶ�ջָ�뼴��
;//* ��    �� : ��cm3�ں���,�����������ı��л��ǿ�������ʵ��  CM3�У�ջ���ɸߵ�ַ��͵�ַ������
;//                 |     ....        |
;//                 |-----------------|
;//                 |     ....        |
;//                 |-----------------|
;//                 |     ....        |
;//                 |-----------------|       |---- �����л�ʱPSP
;//     Low Memory  |     ....        |       |
;//                 |-----------------|       |    |---------------|      |----------------|
;//        ^        |       R4        |  <----|----|--OSTCBStkPtr  |<-----|   (OS_TCB *)   |
;//        ^        |-----------------|            |---------------|      |----------------|
;//        ^        |       R5        |            |               |         OSTCBHighRdy
;//        |        |-----------------|            |---------------|
;//        |        |       R6        |            |               |
;//        |        |-----------------|            |---------------|
;//        |        |       R7        |            |               |
;//        |        |-----------------|            |---------------|
;//        |        |       R8        |                 Task's
;//        |        |-----------------|                 OS_TCB
;//        |        |       R9        |
;//        |        |-----------------|
;//        |        |      R10        |
;//      Stack      |-----------------|
;//      Growth     |      R11        |
;//       = 1       |-----------------|
;//        |        |    R0 = p_arg   |  <-------- �쳣ʱ��PSP (������������ջ)
;//        |        |-----------------|
;//        |        |       R1        |
;//        |        |-----------------|
;//        |        |       R2        |
;//        |        |-----------------|
;//        |        |       R3        |
;//        |        |-----------------|
;//        |        |       R12       |
;//        |        |-----------------|
;//        |        |       LR        |
;//        |        |-----------------|
;//        |        |    PC = task    |
;//        |        |-----------------|
;//        |        |      xPSR       |
;//    High Memory  |-----------------|
;//   
;********************************************************************************************************

OS_CPU_PendSVHandler											;xPSR, PC, LR, R12, R0-R3���Զ�����
    CPSID   I                                                   ;�����л��ڼ���Ҫ���ж� Prevent interruption during context switch
    MRS     R0, PSP                                             ;R0 = PSP PSP is process stack pointer �̶߳�ջָ��
    CBZ     R0, OS_CPU_PendSVHandler_nosave                     ;���PSP==0��ת��OS_CPU_PendSVHandler_nosaveȥִ�� �ڶ�����ĳ�ʼ��ʱPSP����ʼ��Ϊ0  Skip register save the first time
																;����PSP�����0,��ʾ����û�����й�,��ô����Ҫѹջ

    SUBS    R0, R0, #0x20                                       ;R0 -= 0x20 ����R4-R11�������ջ ��32���ֽ�		Save remaining regs r4-11 on process stack
    STM     R0, {R4-R11}										;ѹջR4-R11, ����8���Ĵ��������쳣ʱ�Զ�ѹջ��
    LDR     R1, =OSTCBCur                                       ;��ȡOSTCBCur->OSTCBStkPtr   				R1 = &OSTCBCur    OSTCBCur->OSTCBStkPtr = SP;
    LDR     R1, [R1]											;R1 = *R1 (R1 = OSTCBCur) 
    STR     R0, [R1]                                            ;*R1 = R0 (*OSTCBCur = SP) R0 is SP of process being switched out
																;����ǰ����Ķ�ջ���浽�Լ���������ƿ�
																;OSTCBCur->OSTCBStkPtr = PSP
																;�������д�λ��,�Ѿ������˵�ǰ�����context��


                                                                ; At this point, entire context of process has been saved
OS_CPU_PendSVHandler_nosave
    PUSH    {R14}                                               ; Save LR exc_return value ����R14����Ϊ����Ҫ���ú���  
    LDR     R0, =OSTaskSwHook                                   ; OSTaskSwHook();		   R0 = &OSTaskSwHook 
    BLX     R0													; ����OSTaskSwHook()
    POP     {R14}												; �ָ�R14

    LDR     R0, =OSPrioCur                                      ; OSPrioCur = OSPrioHighRdy; R0 = &OSPrioCur
    LDR     R1, =OSPrioHighRdy									; R1 = &OSPrioHighRdy
    LDRB    R2, [R1]											; R2 = *R1 (R2 = OSPrioHighRdy)
    STRB    R2, [R0]											; *R0 = R2 (OSPrioCur = OSPrioHighRdy)

    LDR     R0, =OSTCBCur                                       ; OSTCBCur  = OSTCBHighRdy;;R0 = &OSTCBCur
    LDR     R1, =OSTCBHighRdy									; R1 = &OSTCBHighRdy
    LDR     R2, [R1]											; R2 = *R1 (R2 = OSTCBHighRdy)
    STR     R2, [R0]											; *R0 = R2 (OSTCBCur = OSTCBHighRdy) ��ʱ [R2] = �������PSP

    LDR     R0, [R2]                                            ;;R0 = *R2 (R0 = OSTCBHighRdy), ��ʱR0���������SP R0 is new process SP; SP = OSTCBHighRdy->OSTCBStkPtr;
    LDM     R0, {R4-R11}                                        ; �������ջSP�ָ�R4-R11 Restore r4-11 from new process stack
    ADDS    R0, R0, #0x20										; ����PSP  R0 += 0x20
    MSR     PSP, R0                                             ; Load PSP with new process SP  PSP = R0��    ���������SP����PSP 
    ORR     LR, LR, #0x04                                       ; Ensure exception return uses process stack  ȷ��LRλ2Ϊ1�����غ�ʹ�ý��̶�ջPSP
    CPSIE   I													;���ж�
    BX      LR                                                  ; Exception return will restore remaining context  �жϷ���

    END
