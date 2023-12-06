	.cpu cortex-m4
	.arch armv7e-m
	.fpu softvfp
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 1
	.eabi_attribute 30, 4
	.eabi_attribute 34, 1
	.eabi_attribute 38, 1
	.eabi_attribute 18, 4
	.file	"ble_srv_common.c"
	.text
.Ltext0:
	.file 1 "C:\\nrf_git\\nrf\\components\\ble\\common\\ble_srv_common.c"
	.section	.text.sd_ble_gatts_characteristic_add,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.type	sd_ble_gatts_characteristic_add, %function
sd_ble_gatts_characteristic_add:
.LVL0:
.LFB211:
	.file 2 "c:\\nrf_git\\nrf\\components\\softdevice\\s112\\headers\\ble_gatts.h"
	.loc 2 501 1 view -0
	@ Naked Function: prologue and epilogue provided by programmer.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	.loc 2 501 1 view .LVU1
	.syntax unified
@ 501 "c:\nrf_git\nrf\components\softdevice\s112\headers\ble_gatts.h" 1
	svc #170
bx r14
@ 0 "" 2
.LVL1:
	.loc 2 501 1 is_stmt 0 view .LVU2
	.thumb
	.syntax unified
.LFE211:
	.size	sd_ble_gatts_characteristic_add, .-sd_ble_gatts_characteristic_add
	.section	.text.sd_ble_gatts_descriptor_add,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.type	sd_ble_gatts_descriptor_add, %function
sd_ble_gatts_descriptor_add:
.LVL2:
.LFB212:
	.loc 2 524 1 is_stmt 1 view -0
	@ Naked Function: prologue and epilogue provided by programmer.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	.loc 2 524 1 view .LVU4
	.syntax unified
@ 524 "c:\nrf_git\nrf\components\softdevice\s112\headers\ble_gatts.h" 1
	svc #171
bx r14
@ 0 "" 2
.LVL3:
	.loc 2 524 1 is_stmt 0 view .LVU5
	.thumb
	.syntax unified
.LFE212:
	.size	sd_ble_gatts_descriptor_add, .-sd_ble_gatts_descriptor_add
	.section	.text.set_security_req,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.type	set_security_req, %function
set_security_req:
.LVL4:
.LFB238:
	.loc 1 91 1 is_stmt 1 view -0
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	.loc 1 94 5 view .LVU7
	.loc 1 94 5 view .LVU8
	.loc 1 94 5 view .LVU9
	movs	r3, #0
	.loc 1 95 5 is_stmt 0 view .LVU10
	subs	r0, r0, #1
.LVL5:
	.loc 1 94 5 view .LVU11
	strb	r3, [r1]
	.loc 1 94 5 is_stmt 1 view .LVU12
	.loc 1 95 5 view .LVU13
	cmp	r0, #4
	bhi	.L3
	tbb	[pc, r0]
.L6:
	.byte	(.L10-.L6)/2
	.byte	(.L9-.L6)/2
	.byte	(.L8-.L6)/2
	.byte	(.L7-.L6)/2
	.byte	(.L5-.L6)/2
	.p2align 1
.L10:
	.loc 1 101 13 view .LVU14
	.loc 1 101 13 view .LVU15
	.loc 1 101 13 view .LVU16
	movs	r3, #17
.L12:
	.loc 1 113 13 is_stmt 0 view .LVU17
	strb	r3, [r1]
	.loc 1 113 13 is_stmt 1 view .LVU18
	.loc 1 114 9 view .LVU19
	.loc 1 116 5 view .LVU20
.L3:
	.loc 1 117 1 is_stmt 0 view .LVU21
	bx	lr
.L9:
	.loc 1 104 13 is_stmt 1 view .LVU22
	.loc 1 104 13 view .LVU23
	.loc 1 104 13 view .LVU24
	movs	r3, #33
	b	.L12
.L8:
	.loc 1 107 13 view .LVU25
	.loc 1 107 13 view .LVU26
	.loc 1 107 13 view .LVU27
	movs	r3, #49
	b	.L12
.L7:
	.loc 1 110 13 view .LVU28
	.loc 1 110 13 view .LVU29
	.loc 1 110 13 view .LVU30
	movs	r3, #18
	b	.L12
.L5:
	.loc 1 113 13 view .LVU31
	.loc 1 113 13 view .LVU32
	.loc 1 113 13 view .LVU33
	movs	r3, #34
	b	.L12
.LFE238:
	.size	set_security_req, .-set_security_req
	.section	.text.ble_srv_is_notification_enabled,"ax",%progbits
	.align	1
	.global	ble_srv_is_notification_enabled
	.syntax unified
	.thumb
	.thumb_func
	.type	ble_srv_is_notification_enabled, %function
ble_srv_is_notification_enabled:
.LVL6:
.LFB234:
	.loc 1 52 1 view -0
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	.loc 1 53 5 view .LVU35
	.loc 1 54 5 view .LVU36
	.loc 1 54 54 is_stmt 0 view .LVU37
	ldrh	r0, [r0]	@ unaligned
.LVL7:
	.loc 1 55 1 view .LVU38
	and	r0, r0, #1
	bx	lr
.LFE234:
	.size	ble_srv_is_notification_enabled, .-ble_srv_is_notification_enabled
	.section	.text.ble_srv_is_indication_enabled,"ax",%progbits
	.align	1
	.global	ble_srv_is_indication_enabled
	.syntax unified
	.thumb
	.thumb_func
	.type	ble_srv_is_ind