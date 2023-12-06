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
	.file	"ble_advdata.c"
	.text
.Ltext0:
	.file 1 "C:\\nrf_git\\nrf\\components\\ble\\common\\ble_advdata.c"
	.section	.text.sd_ble_gap_addr_get,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.type	sd_ble_gap_addr_get, %function
sd_ble_gap_addr_get:
.LVL0:
.LFB13:
	.file 2 "c:\\nrf_git\\nrf\\components\\softdevice\\s112\\headers\\ble_gap.h"
	.loc 2 1362 1 view -0
	@ Naked Function: prologue and epilogue provided by programmer.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	.loc 2 1362 1 view .LVU1
	.syntax unified
@ 1362 "c:\nrf_git\nrf\components\softdevice\s112\headers\ble_gap.h" 1
	svc #109
bx r14
@ 0 "" 2
.LVL1:
	.loc 2 1362 1 is_stmt 0 view .LVU2
	.thumb
	.syntax unified
.LFE13:
	.size	sd_ble_gap_addr_get, .-sd_ble_gap_addr_get
	.section	.text.sd_ble_gap_appearance_get,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.type	sd_ble_gap_appearance_get, %function
sd_ble_gap_appearance_get:
.LVL2:
.LFB26:
	.loc 2 1677 1 is_stmt 1 view -0
	@ Naked Function: prologue and epilogue provided by programmer.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	.loc 2 1677 1 view .LVU4
	.syntax unified
@ 1677 "c:\nrf_git\nrf\components\softdevice\s112\headers\ble_gap.h" 1
	svc #121
bx r14
@ 0 "" 2
.LVL3:
	.loc 2 1677 1 is_stmt 0 view .LVU5
	.thumb
	.syntax unified
.LFE26:
	.size	sd_ble_gap_appearance_get, .-sd_ble_gap_appearance_get
	.section	.text.sd_ble_gap_device_name_get,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.type	sd_ble_gap_device_name_get, %function
sd_ble_gap_device_name_get:
.LVL4:
.LFB30:
	.loc 2 1737 1 is_stmt 1 view -0
	@ Naked Function: prologue and epilogue provided by programmer.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	.loc 2 1737 1 view .LVU7
	.syntax unified
@ 1737 "c:\nrf_git\nrf\components\softdevice\s112\headers\ble_gap.h" 1
	svc #125
bx r14
@ 0 "" 2
.LVL5:
	.loc 2 1737 1 is_stmt 0 view .LVU8
	.thumb
	.syntax unified
.LFE30:
	.size	sd_ble_gap_device_name_get, .-sd_ble_gap_device_name_get
	.section	.text.sd_ble_uuid_encode,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.type	sd_ble_uuid_encode, %function
sd_ble_uuid_encode:
.LVL6:
.LFB210:
	.file 3 "c:\\nrf_git\\nrf\\components\\softdevice\\s112\\headers\\ble.h"
	.loc 3 585 1 is_stmt 1 view -0
	@ Naked Function: prologue and epilogue provided by programmer.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	.loc 3 585 1 view .LVU10
	.syntax unified
@ 585 "c:\nrf_git\nrf\components\softdevice\s112\headers\ble.h" 1
	svc #100
bx r14
@ 0 "" 2
.LVL7:
	.loc 3 585 1 is_stmt 0 view .LVU11
	.thumb
	.syntax unified
.LFE210:
	.size	sd_ble_uuid_encode, .-sd_ble_uuid_encode
	.section	.text.uuid_list_sized_encode,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.type	uuid_list_sized_encode, %function
uuid_list_sized_encode:
.LVL8:
.LFB239:
	.loc 1 256 1 is_stmt 1 view -0
	@ args = 8, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	.loc 1 257 5 view .LVU13
	.loc 1 258 5 view .LVU14
	.loc 1 259 5 view .LVU15
	.loc 1 256 1 is_stmt 0 view .LVU16
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
.LCFI0:
	sub	sp, sp, #20
.LCFI1:
	.loc 1 258 14 view .LVU17
	movs	r6, #0
	.loc 1 256 1 view .LVU18
	ldr	r4, [sp, #56]
	ldrh	fp, [sp, #60]
	.loc 1 259 14 view .LVU19
	ldrh	r10, [r4]
.LVL9:
	.loc 1 260 5 is_stmt 1 view .LVU20
	.loc 1 262 5 view .LVU21
	.loc 1 256 1 is_stmt 0 view .LVU22
	str	r1, [sp, #4]
	mov	r7, r0
	mov	r8, r2
	mov	r5, r3
	.loc 1 262 12 view .LVU23
	mov	r9, r6
.LVL10:
.L6:
	.loc 1 262 19 is_stmt 1 discriminator 1 view .LVU24
	.loc 1 262 32 is_stmt 0 discriminator 1 view .LVU25
	ldrh	r2, [r7]
	.loc 1 262 19 discriminator 1 view .LVU26
	cmp	r2, r9
	bgt	.L12
	.loc 1 299 5 is_stmt 1 view .LVU27
	.loc 1 299 8 is_stmt 0 view .LVU28
	cmp	r6, #0
	bne	.L13
.L14:
	.loc 1 311 12 view .LVU29
	movs	r0, #0
.LVL11:
.L5:
	.loc 1 312 1 view .LVU30
	add	sp, sp, #20
.LCFI2:
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.LVL12:
.L12:
.LCFI3:
.LBB20:
	.loc 1 264 9 is_stmt 1 view .LVU31
	.loc 1 265 9 view .LVU32
	.loc 1 266 9 view .LVU33
	.loc 1 266 20 is_stmt 0 view .LVU34
	ldr	r2, [r7, #4]
	ldr	r0, [r2, r9, lsl #2]	@ unaligned
	str	r0, [sp, #12]
	.loc 1 269 9 is_stmt 1 view .LVU35
	.loc 1 269 20 is_stmt 0 view .LVU36
	movs	r2, #0
	add	r1, sp, #11
	add	r0, sp, #12
	bl	sd_ble_uuid_encode
.LVL13:
	.loc 1 270 9 is_stmt 1 view .LVU37
.LBB21:
	.loc 1 270 9 view .LVU38
	.loc 1 270 9 view .LVU39
	cmp	r0, #0
	bne	.L5
.LBE21:
	.loc 1 270 9 discriminator 2 view .LVU40
	.loc 1 273 9 discriminator 2 view .LVU41
	.loc 1 273 26 is_stmt 0 discriminator 2 view .LVU42
	ldrb	r2, [sp, #11]	@ zero_extendqisi2
	.loc 1 273 12 discriminator 2 view .LVU43
	cmp	r2, r8
	bne	.L8
.LBB22:
	.loc 1 275 13 is_stmt 1 view .LVU44
	.loc 1 278 19 is_stmt 0 view .LVU45
	ldrh	r2, [r4]
	.loc 1 278 30 view .LVU46
	add	r1, r8, r2
	.loc 1 275 21 view .LVU47
	cbz	r6, .L9
.LVL14:
	.loc 1 278 13 is_stmt 1 view .LVU48
	.loc 1 278 16 is_stmt 0 view .LVU49
	cmp	r1, fp
	ble	.L10
.LVL15:
.L17:
	.loc 1 278 16 view .LVU50
.LBE22:
.LBE20:
	.loc 1 306 20 view .LVU51
	movs	r0, #12
	b	.L5
.LVL16:
.L9:
.LBB25:
.LBB24:
	.loc 1 278 13 is_stmt 1 view .LVU52
	.loc 1 278 45 is_stmt 0 view .LVU53
	adds	r1, r1, #2
	.loc 1 278 16 view .LVU54
	cmp	fp, r1
	blt	.L17
	.loc 1 283 13 is_stmt 1 view .LVU55
	.loc 1 286 17 view .LVU56
	.loc 1 286 43 is_stmt 0 view .LVU57
	adds	r2, r2, #1
	uxth	r2, r2
	.loc 1 287 44 view .LVU58
	ldr	r3, [sp, #4]
	.loc 1 286 43 view .LVU59
	strh	r2, [r4]	@ movhi
	.loc 1 287 17 is_stmt 1 view .LVU60
	.loc 1 287 44 is_stmt 0 view .LVU61
	strb	r3, [r5, r2]
	.loc 1 288 17 is_stmt 1 view .LVU62
	.loc 1 288 43 is_stmt 0 view .LVU63
	ldrh	r2, [r4]
	adds	r2, r2, #1
	strh	r2, [r4]	@ movhi
	.loc 1 289 17 is_stmt 1 view .LVU64
.LVL17:
.L10:
	.loc 1 293 13 view .LVU65
	.loc 1 293 81 is_stmt 0 view .LVU66
	ldrh	r2, [r4]
	.loc 1 293 24 view .LVU67
	add	r1, sp, #11
	add	r2, r2, r5
	add	r0, sp, #12
.LVL18:
	.loc 1 293 24 view .LVU68
	bl	sd_ble_uuid_encode
.LVL19:
	.loc 1 294 13 is_stmt 1 view .LVU69
.LBB23:
	.loc 1 294 13 view .LVU70
	.loc 1 294 13 view .LVU71
	cmp	r0, #0
	bne	.L5
.LBE23:
	.loc 1 294 13 discriminator 2 view .LVU72
	.loc 1 295 13 discriminator 2 view .LVU73
	.loc 1 295 23 is_stmt 0 discriminator 2 view .LVU74
	ldrh	r1, [r4]
	ldrb	r2, [sp, #11]	@ zero_extendqisi2
	add	r2, r2, r1
	strh	r2, [r4]	@ movhi
	movs	r6, #1
.LVL20:
.L8:
	.loc 1 295 23 discriminator 2 view .LVU75
.LBE24:
.LBE25:
	.loc 1 262 45 is_stmt 1 view .LVU76
	add	r9, r9, #1
.LVL21:
	.loc 1 262 45 is_stmt 0 view .LVU77
	b	.L6
.LVL22:
.L13:
	.loc 1 302 9 is_stmt 1 view .LVU78
	.loc 1 302 16 is_stmt 0 view .LVU79
	ldrh	r3, [r4]
	subs	r3, r3, #1
	sub	r3, r3, r10
	uxth	r3, r3
.LVL23:
	.loc 1 304 9 is_stmt 1 view .LVU80
	.loc 1 304 12 is_stmt 0 view .LVU81
	cmp	r3, #255
	bhi	.L17
	.loc 1 308 9 is_stmt 1 view .LVU82
	.loc 1 308 37 is_stmt 0 view .LVU83
	strb	r3, [r5, r10]
	b	.L14
.LFE239:
	.size	uuid_list_sized_encode, .-uuid_list_sized_encode
	.section	.text.uuid_list_encode,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.type	uuid_list_encode, %function
uuid_list_encode:
.LVL24:
.LFB240:
	.loc 1 321 1 is_stmt 1 view -0
	@ args = 8, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	.loc 1 321 1 is_stmt 0 view .LVU85
	push	{r0, r1, r4, r5, r6, r7, r8, lr}
.LCFI4:
	.loc 1 321 1 view .LVU86
	ldr	r7, [sp, #32]
	ldrh	r8, [sp, #36]
	mov	r5, r2
	.loc 1 325 16 view .LVU87
	strd	r7, r8, [sp]
	movs	r2, #2
.LVL25:
	.loc 1 321 1 view .LVU88
	mov	r4, r0
	.loc 1 322 5 is_stmt 1 view .LVU89
	.loc 1 325 5 view .LVU90
	.loc 1 321 1 is_stmt 0 view .LVU91
	mov	r6, r3
	.loc 1 325 16 view .LVU92
	bl	uuid_list_sized_encode
.LVL26:
	.loc 1 331 5 is_stmt 1 view .LVU93
.LBB31:
	.loc 1 331 5 view .LVU94
	.loc 1 331 5 view .LVU95
	cbnz	r0, .L18
.LVL27:
	.loc 1 331 5 is