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
	.file	"SEGGER_RTT.c"
	.text
.Ltext0:
	.file 1 "C:\\nrf_git\\nrf\\external\\segger_rtt\\SEGGER_RTT.c"
	.section	.text._GetAvailWriteSpace,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.type	_GetAvailWriteSpace, %function
_GetAvailWriteSpace:
.LVL0:
.LFB210:
	.loc 1 485 66 view -0
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	.loc 1 486 3 view .LVU1
	.loc 1 487 3 view .LVU2
	.loc 1 488 3 view .LVU3
	.loc 1 493 3 view .LVU4
	.loc 1 493 9 is_stmt 0 view .LVU5
	ldr	r3, [r0, #16]
.LVL1:
	.loc 1 494 3 is_stmt 1 view .LVU6
	.loc 1 494 9 is_stmt 0 view .LVU7
	ldr	r2, [r0, #12]
.LVL2:
	.loc 1 495 3 is_stmt 1 view .LVU8
	.loc 1 495 6 is_stmt 0 view .LVU9
	cmp	r3, r2
	.loc 1 496 5 is_stmt 1 view .LVU10
	.loc 1 496 7 is_stmt 0 view .LVU11
	itt	ls
	ldrls	r1, [r0, #8]
	addls	r3, r3, r1
.LVL3:
	.loc 1 498 5 is_stmt 1 view .LVU12
	.loc 1 498 7 is_stmt 0 view .LVU13
	subs	r3, r3, #1
	subs	r0, r3, r2
.LVL4:
	.loc 1 500 3 is_stmt 1 view .LVU14
	.loc 1 501 1 is_stmt 0 view .LVU15
	bx	lr
.LFE210:
	.size	_GetAvailWriteSpace, .-_GetAvailWriteSpace
	.section	.text._WriteNoCheck,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.type	_WriteNoCheck, %function
_WriteNoCheck:
.LVL5:
.LFB208:
	.loc 1 399 94 is_stmt 1 view -0
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	.loc 1 400 3 view .LVU17
	.loc 1 401 3 view .LVU18
	.loc 1 402 3 view .LVU19
	.loc 1 407 3 view .LVU20
	.loc 1 399 94 is_stmt 0 view .LVU21
	push	{r4, r5, r6, r7, r8, lr}
.LCFI0:
	.loc 1 408 14 view .LVU22
	ldrd	r8, r4, [r0, #8]
	.loc 1 399 94 view .LVU23
	mov	r5, r0
	.loc 1 408 7 view .LVU24
	sub	r6, r8, r4
.LVL6:
	.loc 1 409 3 is_stmt 1 view .LVU25
	.loc 1 421 5 is_stmt 0 view .LVU26
	ldr	r0, [r0, #4]
.LVL7:
	.loc 1 409 6 view .LVU27
	cmp	r6, r2
	.loc 1 421 5 view .LVU28
	add	r0, r0, r4
	.loc 1 399 94 view .LVU29
	mov	r7, r1
	.loc 1 422 26 view .LVU30
	add	r4, r4, r2
	.loc 1 409 6 view .LVU31
	bls	.L5
.LVL8:
.L7:
	.loc 1 444 5 view .LVU32
	bl	memcpy
.LVL9:
	.loc 1 445 5 is_stmt 1 view .LVU33
	.loc 1 422 18 is_stmt 0 view .LVU34
	str	r4, [r5, #12]
	.loc 1 448 1 view .LVU35
	pop	{r4, r5, r6, r7, r8, pc}
.LVL10:
.L5:
	.loc 1 441 5 is_stmt 1 view .LVU36
	.loc 1 442 5 view .LVU37
	mov	r2, r6
.LVL11:
	.loc 1 443 20 is_stmt 0 view .LVU38
	sub	r4, r4, r8
	.loc 1 442 5 view .LVU39
	bl	memcpy
.LVL12:
	.loc 1 443 5 is_stmt 1 view .LVU40
	.loc 1 444 5 view .LVU41
	ldr	r0, [r5, #4]
	mov	r2, r4
	adds	r1, r7, r6
	b	.L7
.LFE208:
	.size	_WriteNoCheck, .-_WriteNoCheck
	.section	.text._WriteBlocking,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.type	_WriteBlocking, %function
_WriteBlocking:
.LVL13:
.LFB207:
	.loc 1 335 101 view -0
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	.loc 1 336 3 view .LVU43
	.loc 1 337 3 view .LVU44
	.loc 1 338 3 view .LVU45
	.loc 1 339 3 view .LVU46
	.loc 1 346 3 view .LVU47
	.loc 1 347 3 view .LVU48
	.loc 1 335 101 is_stmt 0 view .LVU49
	push	{r3, r4, r5, r6, r7, r8, r9, lr}
.LCFI1:
	.loc 1 347 9 view .LVU50
	ldr	r5, [r0, #12]
.LVL14:
	.loc 1 335 101 view .LVU51
	mov	r6, r0
	mov	r9, r1
	mov	r7, r2
	.loc 1 346 19 view .LVU52
	mov	r8, #0
.LVL15:
.L12:
	.loc 1 348 3 is_stmt 1 view .LVU53
	.loc 1 349 5 view .LVU54
	.loc 1 349 11 is_stmt 0 view .LVU55
	ldr	r3, [r6, #16]
.LVL16:
	.loc 1 350 5 is_stmt 1 view .LVU56
	.loc 1 353 30 is_stmt 0 view .LVU57
	ldr	r4, [r6, #8]
	.loc 1 366 5 view .LVU58
	ldr	r0, [r6, #4]
	.loc 1 350 8 view .LVU59
	cmp	r5, r3
	.loc 1 351 7 is_stmt 1 view .LVU60
	.loc 1 353 7 view .LVU61
	.loc 1 353 23 is_stmt 0 view .LVU62
	it	cs
	addcs	r3, r3, r4
.LVL17:
	.loc 1 355 23 view .LVU63
	subs	r4, r4, r5
	.loc 1 356 21 view .LVU64
	cmp	r4, r7
	.loc 1 353 23 view .LVU65
	add	r3, r3, #-1
	.loc 1 356 21 view .LVU66
	it	cs
	movcs	r4, r7
	.loc 1 353 23 view .LVU67
	subs	r3, r3, r5
.LVL18:
	.loc 1 355 5 is_stmt 1 view .LVU68
	.loc 1 356 5 view .LVU69
	.loc 1 356 21 is_stmt 0 view .LVU70
	cmp	r4, r3
	it	cs
	movcs	r4, r3
.LVL19:
	.loc 1 366 5 is_stmt 1 view .LVU71
	mov	r1, r9
	add	r0, r0, r5
	mov	r2, r4
	bl	memcpy
.LVL20:
	.loc 1 367 5 view .LVU72
	.loc 1 372 8 is_stmt 0 view .LVU73
	ldr	r3, [r6, #8]
	.loc 1 370 21 view .LVU74
	add	r5, r5, r4
.LVL21:
	.loc 1 373 13 view .LVU75
	cmp	r3, r5
	it	eq
	moveq	r5, #0
	.loc 1 376 12 view .LVU76
	subs	r7, r7, r4
.LVL22:
	.loc 1 367 21 view .LVU77
	add	r8, r8, r4
.LVL23:
	.loc 1 368 5 is_stmt 1 view .LVU78
	.loc 1 368 21 is_stmt 0 view .LVU79
	add	r9, r9, r4
.LVL24:
	.loc 1 369 5 is_stmt 1 view .LVU80
	.loc 1 370 5 view .LVU81
	.loc 1 372 5 view .LVU82
	.loc 1 375 5 view .LVU83
	.loc 1 375 18 is_stmt 0 view .LVU84
	str	r5, [r6, #12]
	.loc 1 376 12 is_stmt 1 view .LVU85
	bne	.L12
	.loc 1 378 3 view .LVU86
	.loc 1 379 1 is_stmt 0 view .LVU87
	mov	r0, r8
	pop	{r3, r4, r5, r6, r7, r8, r9, pc}
	.loc 1 379 1 view .LVU88
.LFE207:
	.size	_WriteBlocking, .-_WriteBlocking
	.section	.rodata._DoInit.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"Terminal\000"
.LC1:
	.ascii	"RTT\000"
.LC2:
	.ascii	"SEGGER\000"
	.section	.text._DoInit,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.type	_DoInit, %function
_DoInit:
.LFB206:
	.loc 1 280 27 is_stmt 1 view -0
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	.loc 1 281 3 view .LVU90
	.loc 1 285 3 view .LVU91
.LVL25:
	.loc 1 286 3 view .LVU92
	.loc 1 280 27 is_stmt 0 view .LVU93
	push	{r4, lr}
.LCFI2:
	.loc 1 286 25 view .LVU94
	ldr	r4, .L16
	.loc 1 291 27 view .LVU95
	ldr	r2, .L16+4
	str	r2, [r4, #24]
	.loc 1 286 25 view .LVU96
	movs	r3, #2
	.loc 1 287 25 view .LVU97
	strd	r3, r3, [r4, #16]
	.loc 1 291 3 is_stmt 1 view .LVU98
	.loc 1 292 3 view .LVU99
	.loc 1 292 27 is_stmt 0 view .LVU100
	ldr	r3, .L16+8
	str	r3, [r4, #28]
	.loc 1 293 3 is_stmt 1 view .LVU101
	.loc 1 300 29 is_stmt 0 view .LVU102
	str	r2, [r4, #72]
	.loc 1 293 27 view .LVU103
	mov	r3, #512
	.loc 1 301 29 view .LVU104
	ldr	r2, .L16+12
	.loc 1 311 3 view .LVU105
	ldr	r1, .L16+16
	.loc 1 293 27 view .LVU106
	str	r3, [r4, #32]
	.loc 1 294 3 is_stmt 1 view .LVU107
	.loc 1 301 29 is_stmt 0 view .LVU108
	str	r2, [r4, #76]
	.loc 1 294 27 view .LVU109
	movs	r3, #0
	.loc 1 302 29 view .LVU110
	movs	r2, #16
	.loc 1 311 3 view .LVU111
	adds	r0, r4, #7
	.loc 1 294 27 view .LVU112
	str	r3, [r4, #40]
	.loc 1 295 3 is_stmt 1 view .LVU113
	.loc 1 295 27 is_stmt 0 view .LVU114
	str	r3, [r4, #36]
	.loc 1 296 3 is_stmt 1 view .LVU115
	.loc 1 296 27 is_stmt 0 view .LVU116
	str	r3, [r4, #44]
	.loc 1 300 3 is_stmt 1 view .LVU117
	.loc 1 301 3 view .LVU118
	.loc 1 302 3 view .LVU119
	.loc 1 303 29 is_stmt 0 view .LVU120
	str	r3, [r4, #88]
	.loc 1 304 29 view .LVU121
	str	r3, [r4, #84]
	.loc 1 305 29 view .LVU122
	str	r3, [r4, #92]
	.loc 1 302 29 view .LVU123
	str	r2, [r4, #80]
	.loc 1 303 3 is_stmt 1 view .LVU124
	.loc 1 304 3 view .LVU125
	.loc 1 305 3 view .LVU126
	.loc 1 311 3 view .LVU127
	bl	strcpy
.LVL26:
	.loc 1 312 3 view .LVU128
	ldr	r1, .L16+20
	mov	r0, r4
	bl	strcpy
.LVL27:
	.loc 1 313 3 view .LVU129
	.loc 1 313 14 is_stmt 0 view .LVU130
	movs	r3, #32
	strb	r3, [r4, #6]
	.loc 1 314 1 view .LVU131
	pop	{r4, pc}
.L17:
	.align	2
.L16:
	.word	_SEGGER_RTT
	.word	.LC0
	.word	_acUpBuffer
	.word	_acDownBuffer
	.word	.LC1
	.word	.LC2
.LFE206:
	.size	_DoInit, .-_DoInit
	.section	.text._PostTerminalSwitch.constprop.0,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.type	_PostTerminalSwitch.constprop.0, %function
_PostTerminalSwitch.constprop.0:
.LVL28:
.LFB236:
	.loc 1 463 13 is_stmt 1 view -0
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	.loc 1 464 3 view .LVU133
	.loc 1 466 3 view .LVU134
	.loc 1 463 13 is_stmt 0 view .LVU135
	push	{r0, r1, r2, lr}
.LCFI3:
	.loc 1 466 9 view .LVU136
	movs	r3, #255
	strb	r3, [sp, #4]
	.loc 1 467 3 is_stmt 1 view .LVU137
	.loc 1 467 23 is_stmt 0 view .LVU138
	ldr	r3, .L19
	.loc 1 468 3 view .LVU139
	movs	r2, #2
	.loc 1 467 9 view .LVU140
	ldrb	r3, [r3, r0]	@ zero_extendqisi2
	.loc 1 468 3 view .LVU141
	ldr	r0, .L19+4
.LVL29:
	.loc 1 467 9 view .LVU142
	strb	r3, [sp, #5]
	.loc 1 468 3 is_stmt 1 view .LVU143
	add	r1, sp, #4
	bl	_WriteBlocking
.LVL30:
	.loc 1 469 1 is_stmt 0 view .LVU144
	add	sp, sp, #12
.LCFI4:
	@ sp needed
	ldr	pc, [sp], #4
.L20:
	.align	2
.L19:
	.word	_aTerminalId
	.word	_SEGGER_RTT+24
.LFE236:
	.size	_PostTerminalSwitch.constprop.0, .-_PostTerminalSwitch.constprop.0
	.section	.text.SEGGER_RTT_ReadNoLock,"ax",%progbits
	.align	1
	.global	SEGGER_RTT_ReadNoLock
	.syntax unified
	.thumb
	.thumb_func
	.type	SEGGER_RTT_ReadNoLock, %function
SEGGER_RTT_ReadNoLock:
.LVL31:
.LFB211:
	.loc 1 526 88 is_stmt 1 view -0
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	.loc 1 526 88 is_stmt 0 view .LVU146
	push	{r3, r4, r5, r6, r7, r8, r9, r10, fp, lr}
.LCFI5:
	.loc 1 537 3 view .LVU147
	ldr	r8, .L33
	ldrb	r3, [r8]	@ zero_extendqisi2
	.loc 1 526 88 view .LVU148
	mov	r10, r0
	mov	r9, r1
	.loc 1 527 3 is_stmt 1 view .LVU149
	.loc 1 528 3 view .LVU150
	.loc 1 529 3 view .LVU151
	.loc 1 530 3 view .LVU152
	.loc 1 531 3 view .LVU153
	.loc 1 532 3 view .LVU154
	.loc 1 537 3 view .LVU155
	.loc 1 537 3 view .LVU156
	.loc 1 526 88 is_stmt 0 view .LVU157
	mov	r6, r2
	.loc 1 537 3 view .LVU158
	cbnz	r3, .L22
	.loc 1 537 3 is_stmt 1 discriminator 1 view .LVU159
	bl	_DoInit
.LVL32:
.L22:
	.loc 1 537 3 discriminator 3 view .LVU160
	.loc 1 538 3 discriminator 3 view .LVU161
	.loc 1 539 3 discriminator 3 view .LVU162
	.loc 1 540 3 discriminator 3 view .LVU163
	.loc 1 540 9 is_stmt 0 discriminator 3 view .LVU164
	movs	r7, #24
	mla	r7, r7, r10, r8
	ldr	r5, [r7, #88]
.LVL33:
	.loc 1 541 3 is_stmt 1 discriminator 3 view .LVU165
	.loc 1 541 9 is_stmt 0 discriminator 3 view .LVU166
	ldr	fp, [r7, #84]
.LVL34:
	.loc 1 542 3 is_stmt 1 discriminator 3 view .LVU167
	.loc 1 546 3 discriminator 3 view .LVU168
	.loc 1 546 6 is_stmt 0 discriminator 3 view .LVU169
	cmp	r5, fp
	bls	.L26
	.loc 1 547 5 is_stmt 1 view .LVU170
	.loc 1 547 17 is_stmt 0 view .LVU171
	ldr	r4, [r7, #80]
	.loc 1 558 5 view .LVU172
	ldr	r1, [r7, #76]
	.loc 1 547 17 view .LVU173
	subs	r4, r4, r5
.LVL35:
	.loc 1 548 5 is_stmt 1 view .LVU174
	.loc 1 548 17 is_stmt 0 view .LVU175
	cmp	r4, r6
	it	cs
	movcs	r4, r6
.LVL36:
	.loc 1 558 5 is_stmt 1 view .LVU176
	add	r1, r1, r5
	mov	r0, r9
	mov	r2, r4
	bl	memcpy
.LVL37:
	.loc 1 559 5 view .LVU177
	.loc 1 560 5 view .LVU178
	.loc 1 567 8 is_stmt 0 view .LVU179
	ldr	r3, [r7, #80]
	.loc 1 562 18 view .LVU180
	add	r5, r5, r4
.LVL38:
	.loc 1 560 18 view .LVU181
	add	r9, r9, r4
.LVL39:
	.loc 1 561 5 is_stmt 1 view .LVU182
	.loc 1 561 18 is_stmt 0 view .LVU183
	subs	r6, r6, r4
.LVL40:
	.loc 1 562 5 is_stmt 1 view .LVU184
	.loc 1 567 5 view .LVU185
	.loc 1 568 13 is_stmt 0 view .LVU186
	cmp	r3, r5
	it	eq
	moveq	r5, #0
.LVL41:
.L23:
	.loc 1 574 3 is_stmt 1 view .LVU187
	.loc 1 574 15 is_stmt 0 view .LVU188
	sub	fp, fp, r5
.LVL42:
	.loc 1 575 3 is_stmt 1 view .LVU189
	.loc 1 575 15 is_stmt 0 view .LVU190
	cmp	r6, fp
	it	cs
	movcs	r6, fp
.LVL43:
	.loc 1 576 3 is_stmt 1 view .LVU191
	.loc 1 576 6 is_stmt 0 view .LVU192
	cbz	r6, .L24
	.loc 1 586 5 is_stmt 1 view .LVU193
	movs	r3, #24
	mla	r3, r3, r10, r8
	mov	r2, r6
	ldr	r1, [r3, #76]
	mov	r0, r9
	add	r1, r1, r5
	bl	memcpy
.LVL44:
	.loc 1 587 5 view .LVU194
	.loc 1 587 18 is_stmt 0 view .LVU195
	add	r4, r4, r6
.LVL45:
	.loc 1 588 5 is_stmt 1 view .LVU196
	.loc 1 589 5 view .LVU197
	.loc 1 590 5 view .LVU198
	.loc 1 590 18 is_stmt 0 view .LVU199
	add	r5, r5, r6
.LVL46:
.L24:
	.loc 1 593 3 is_stmt 1 view .LVU200
	.loc 1 593 6 is_stmt 0 view .LVU201
	cbz	r4, .L21
	.loc 1 594 5 is_stmt 1 view .LVU202
	.loc 1 594 18 is_stmt 0 view .LVU203
	movs	r3, #24
	mla	r8, r3, r10, r8
	str	r5, [r8, #88]
	.loc 1 597 3 is_stmt 1 view .LVU204
.L21:
	.loc 1 598 1 is_stmt 0 view .LVU205
	mov	r0, r4
	pop	{r3, r4, r5, r6, r7, r8, r9, r10, fp, pc}
.LVL47:
.L26:
	.loc 1 542 16 view .LVU206
	movs	r4, #0
	b	.L23
.L34:
	.align	2
.L33:
	.word	_SEGGER_RTT
.LFE211:
	.size	SEGGER_RTT_ReadNoLock, .-SEGGER_RTT_ReadNoLock
	.section	.text.SEGGER_RTT_Read,"ax",%progbits
	.align	1
	.global	SEGGER_RTT_Read
	.syntax unified
	.thumb
	.thumb_func
	.type	SEGGER_RTT_Read, %function
SEGGER_RTT_Read:
.LVL48:
.LFB212:
	.loc 1 616 84 is_stmt 1 view -0
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	.loc 1 617 3 view .LVU208
.LBB2:
	.loc 1 619 3 view .LVU209
.LBE2:
	.loc 1 616 84 is_stmt 0 view .LVU210
	push	{r0, r1, r4, r5, r6, lr}
.LCFI6:
.LBB3:
	.loc 1 619 3 view .LVU211
	movs	r3, #0
.LBE3:
	.loc 1 616 84 view .LVU212
	mov	r4, r0
	mov	r5, r1
	mov	r6, r2
.LBB4:
	.loc 1 619 3 view .LVU213
	add	r0, sp, #7
.LVL49:
	.loc 1 619 3 view .LVU214
	strb	r3, [sp, #7]
	.loc 1 619 3 is_stmt 1 view .LVU215
	bl	app_util_critical_region_enter
.LVL50:
	.loc 1 619 20 view .LVU216
	.loc 1 623 3 view .LVU217
	.loc 1 623 18 is_stmt 0 view .LVU218
	mov	r2, r6
	mov	r1, r5
	mov	r0, r4
	bl	SEGGER_RTT_ReadNoLock
.LVL51:
	mov	r4, r0
.LVL52:
	.loc 1 627 3 is_stmt 1 view .LVU219
	ldrb	r0, [sp, #7]	@ zero_extendqisi2
.LVL53:
	.loc 1 627 3 is_stmt 0 view .LVU220
	bl	app_util_critical_region_exit
.LVL54:
.LBE4:
	.loc 1 627 22 is_stmt 1 view .LVU221
	.loc 1 629 3 view .LVU222
	.loc 1 630 1 is_stmt 0 view .LVU223
	mov	r0, r4
	add	sp, sp, #8
.LCFI7:
	@ sp needed
	pop	{r4, r5, r6, pc}
	.loc 1 630 1 view .LVU224
.LFE212:
	.size	SEGGER_RTT_Read, .-SEGGER_RTT_Read
	.section	.text.SEGGER_RTT_WriteWithOverwriteNoLock,"ax",%progbits
	.align	1
	.global	SEGGER_RTT_WriteWithOverwriteNoLock
	.syntax unified
	.thumb
	.thumb_func
	.type	SEGGER_RTT_WriteWithOverwriteNoLock, %function
SEGGER_RTT_WriteWithOverwriteNoLock:
.LVL55:
.LFB213:
	.loc 1 655 104 is_stmt 1 view -0
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	.loc 1 656 3 view .LVU226
	.loc 1 657 3 view .LVU227
	.loc 1 658 3 view .LVU228
	.loc 1 663 3 view .LVU229
	.loc 1 655 104 is_stmt 0 view .LVU230
	push	{r3, r4, r5, r6, r7, r8, r9, r10, fp, lr}
.LCFI8:
	.loc 1 671 12 view .LVU231
	ldr	r6, .L49
	.loc 1 655 104 view .LVU232
	mov	r4, r2
	.loc 1 671 12 view .LVU233
	movs	r2, #24
.LVL56:
	.loc 1 671 12 view .LVU234
	mla	r2, r2, r0, r6
	.loc 1 655 104 view .LVU235
	mov	r7, r1
.LVL57:
	.loc 1 667 3 is_stmt 1 view .LVU236
	.loc 1 671 3 view .LVU237
	.loc 1 671 28 is_stmt 0 view .LVU238
	ldr	r3, [r2, #40]
	.loc 1 671 12 view .LVU239
	ldr	r1, [r2, #36]
.LVL58:
	.loc 1 671 6 view .LVU240
	cmp	r1, r3
	.loc 1 655 104 view .LVU241
	mov	r5, r0
	.loc 1 671 6 view .LVU242
	bne	.L37
	.loc 1 672 5 is_stmt 1 view .LVU243
	.loc 1 672 11 is_stmt 0 view .LVU244
	ldr	r3, [r2, #32]
	subs	r3, r3, #1
.LVL59:
.L38:
	.loc 1 678 3 is_stmt 1 view .LVU245
	.loc 1 678 6 is_stmt 0 view .LVU246
	cmp	r3, r4
	bcc	.L40
.LVL60:
.L44:
	.loc 1 687 3 is_stmt 1 view .LVU247
	.loc 1 687 16 is_stmt 0 view .LVU248
	mov	r10, #24
	mla	r3, r10, r5, r6
	.loc 1 701 7 view .LVU249
	mla	fp, r5, r10, r10
	.loc 1 687 9 view .LVU250
	ldr	r3, [r3, #32]
	sub	r8, r3, r1
.LVL61:
.L41:
	.loc 1 688 3 is_stmt 1 view .LVU251
	.loc 1 689 5 view .LVU252
	.loc 1 701 7 is_stmt 0 view .LVU253
	add	r3, r6, fp
	mla	r9, r10, r5, r6
	ldr	r0, [r3, #4]
	ldr	r3, [r9, #36]
	.loc 1 689 8 view .LVU254
	cmp	r4, r8
	.loc 1 701 7 view .LVU255
	add	r0, r0, r3
	.loc 1 689 8 view .LVU256
	bcs	.L45
	.loc 1 701 7 is_stmt 1 view .LVU257
	mov	r2, r4
	mov	r1, r7
	bl	memcpy
.LVL62:
	.loc 1 702 7 view .LVU258
	.loc 1 702 20 is_stmt 0 view .LVU259
	ldr	r3, [r9, #36]
	add	r3, r3, r4
	str	r3, [r9, #36]
	.loc 1 704 7 is_stmt 1 view .LVU260
.L36:
	.loc 1 725 1 is_stmt 0 view .LVU261
	pop	{r3, r4, r5, r6, r7, r8, r9, r10, fp, pc}
.LVL63:
.L37:
	.loc 1 673 10 is_stmt 1 view .LVU262
	.loc 1 673 35 is_stmt 0 view .LVU263
	ldr	r3, [r2, #40]
	.loc 1 673 13 view .LVU264
	cmp	r1, r3
	.loc 1 674 18 view .LVU265
	ldr	r3, [r2, #40]
	.loc 1 674 5 is_stmt 1 view .LVU266
	.loc 1 676 5 view .LVU267
	.loc 1 676 11 is_stmt 0 view .LVU268
	itt	cs
	ldrcs	r2, [r2, #32]
	addcs	r3, r3, r2
	subs	r3, r3, #1
	subs	r3, r3, r1
.LVL64:
	.loc 1 676 11 view .LVU269
	b	.L38
.L40:
	.loc 1 679 5 is_stmt 1 view .LVU270
	.loc 1 679 10 is_stmt 0 view .LVU271
	movs	r2, #24
	mla	r2, r2, r5, r6
	ldr	r0, [r2, #40]
.LVL65:
	.loc 1 679 18 view .LVU272
	add	r0, r0, r4
	subs	r3, r0, r3
.LVL66:
	.loc 1 680 33 view .LVU273
	ldr	r0, [r2, #32]
	.loc 1 679 18 view .LVU274
	str	r3, [r2, #40]
	.loc 1 680 5 is_stmt 1 view .LVU275
.L42:
	.loc 1 680 25 view .LVU276
	.loc 1 680 17 is_stmt 0 view .LVU277
	ldr	r3, [r2, #40]
	.loc 1 680 25 view .LVU278
	cmp	r3, r0
	bcc	.L44
	.loc 1 681 7 is_stmt 1 view .LVU279
	.loc 1 681 12 is_stmt 0 view .LVU280
	ldr	r3, [r2, #40]
	.loc 1 681 20 view .LVU281
	subs	r3, r3, r0
	str	r3, [r2, #40]
	b	.L42
.LVL67:
.L45:
	.loc 1 717 7 is_stmt 1 view .LVU282
	mov	r2, r8
	mov	r1, r7
	bl	memcpy
.LVL68:
	.loc 1 718 7 view .LVU283
	.loc 1 719 20 is_stmt 0 view .LVU284
	movs	r3, #0
	str	r3, [r9, #36]
	.loc 1 722 13 view .LVU285
	ldr	r3, [r9, #32]
	.loc 1 720 16 view .LVU286
	sub	r4, r4, r8
.LVL69:
	.loc 1 718 13 view .LVU287
	add	r7, r7, r8
.LVL70:
	.loc 1 719 7 is_stmt 1 view .LVU288
	.loc 1 720 7 view .LVU289
	.loc 1 722 7 view .LVU290
	.loc 1 722 13 is_stmt 0 view .LVU291
	add	r8, r3, #-1
.LVL71:
	.loc 1 724 12 is_stmt 1 view .LVU292
	cmp	r4, #0
	bne	.L41
	b	.L36
.L50:
	.align	2
.L49:
	.word	_SEGGER_RTT
.LFE213:
	.size	SEGGER_RTT_WriteWithOverwriteNoLock, .-SEGGER_RTT_WriteWithOverwriteNoLock
	.section	.text.SEGGER_RTT_WriteSkipNoLock,"ax",%progbits
	.align	1
	.global	SEGGER_RTT_WriteSkipNoLock
	.syntax unified
	.thumb
	.thumb_func
	.type	SEGGER_RTT_WriteSkipNoLock, %function
SEGGER_RTT_WriteSkipNoLock:
.LVL72:
.LFB214:
	.loc 1 751 99 view -0
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	.loc 1 752 3 view .LVU294
	.loc 1 753 3 view .LVU295
	.loc 1 754 3 view .LVU296
	.loc 1 755 3 view .LVU297
	.loc 1 756 3 view .LVU298
	.loc 1 757 3 view .LVU299
	.loc 1 762 3 view .LVU300
	.loc 1 766 3 view .LVU301
	.loc 1 767 3 view .LVU302
	.loc 1 751 99 is_stmt 0 view .LVU303
	push	{r3, r4, r5, r6, r7, r8, r9, r10, fp, lr}
.LCFI9:
	.loc 1 767 9 view .LVU304
	ldr	r8, .L61
	movs	r3, #24
	mla	r4, r3, r0, r8
	.loc 1 751 99 view .LVU305
	mov	r7, r0
	.loc 1 768 9 view .LVU306
	ldr	r5, [r4, #36]
	.loc 1 767 9 view .LVU307
	ldr	r0, [r4, #40]
.LVL73:
	.loc 1 768 3 is_stmt 1 view .LVU308
	.loc 1 782 3 view .LVU309
	.loc 1 782 6 is_stmt 0 view .LVU310
	cmp	r0, r5
	.loc 1 751 99 view .LVU311
	mov	r10, r1
	mov	r6, r2
	.loc 1 782 6 view .LVU312
	bhi	.L52
	.loc 1 786 5 is_stmt 1 view .LVU313
	.loc 1 786 18 is_stmt 0 view .LVU314
	ldr	fp, [r4, #32]
	.loc 1 786 38 view .LVU315
	sub	r9, fp, r5
	.loc 1 786 11 view .LVU316
	add	r4, r9, #-1
.LVL74:
	.loc 1 787 5 is_stmt 1 view .LVU317
	.loc 1 787 8 is_stmt 0 view .LVU318
	cmp	r2, r4
	bhi	.L53
.LVL75:
.L60:
	.loc 1 860 7 is_stmt 1 view .LVU319
	mla	r3, r7, r3, r3
	add	r3, r3, r8
	ldr	r0, [r3, #4]
	add	r0, r0, r5
	bl	memcpy
.LVL76:
	.loc 1 861 7 view .LVU320
	.loc 1 861 28 is_stmt 0 view .LVU321
	add	r5, r5, r6
.LVL77:
	.loc 1 863 7 is_stmt 1 view .LVU322
	.loc 1 863 14 is_stmt 0 view .LVU323
	b	.L54
.LVL78:
.L53:
	.loc 1 804 5 is_stmt 1 view .LVU324
	.loc 1 808 5 view .LVU325
	.loc 1 804 11 is_stmt 0 view .LVU326
	add	r0, r0, r4
.LVL79:
	.loc 1 808 8 view .LVU327
	cmp	r2, r0
	bhi	.L58
	.loc 1 812 7 is_stmt 1 view .LVU328
.LVL80:
	.loc 1 813 7 view .LVU329
	.loc 1 796 7 is_stmt 0 view .LVU330
	mla	r3, r7, r3, r3
	add	r4, r8, r3
	.loc 1 813 10 view .LVU331
	cmp	r9, r2
	.loc 1 822 9 view .LVU332
	ldr	r0, [r4, #4]
.LVL81:
	.loc 1 822 9 view .LVU333
	add	r0, r0, r5
	.loc 1 797 28 view .LVU334
	add	r5, r5, r2
.LVL82:
	.loc 1 813 10 view .LVU335
	bls	.L56
.LVL83:
.L59:
	.loc 1 843 9 view .LVU336
	bl	memcpy
.LVL84:
	.loc 1 844 9 is_stmt 1 view .LVU337
.L54:
	.loc 1 844 22 is_stmt 0 view .LVU338
	movs	r3, #24
	mla	r3, r3, r7, r8
	.loc 1 799 14 view .LVU339
	movs	r0, #1
	.loc 1 844 22 view .LVU340
	str	r5, [r3, #36]
.L51:
	.loc 1 870 1 view .LVU341
	pop	{r3, r4, r5, r6, r7, r8, r9, r10, fp, pc}
.LVL85:
.L56:
	.loc 1 842 9 is_stmt 1 view .LVU342
	mov	r2, r9
.LVL86:
	.loc 1 843 9 is_stmt 0 view .LVU343
	sub	r5, r5, fp
	.loc 1 842 9 view .LVU344
	bl	memcpy
.LVL87:
	.loc 1 843 9 is_stmt 1 view .LVU345
	ldr	r0, [r4, #4]
	mov	r2, r5
	add	r1, r10, r9
	b	.L59
.LVL88:
.L52:
	.loc 1 850 5 view .LVU346
	.loc 1 851 5 view .LVU347
	.loc 1 850 11 is_stmt 0 view .LVU348
	subs	r0, r0, #1
.LVL89:
	.loc 1 850 11 view .LVU349
	subs	r0, r0, r5
.LVL90:
	.loc 1 851 8 view .LVU350
	cmp	r0, r2
	bcs	.L60
.LVL91:
.L58:
	.loc 1 869 10 view .LVU351
	movs	r0, #0
	b	.L51
.L62:
	.align	2
.L61:
	.word	_SEGGER_RTT
.LFE214:
	.size	SEGGER_RTT_WriteSkipNoLock, .-SEGGER_RTT_WriteSkipNoLock
	.section	.text.SEGGER_RTT_WriteNoLock,"ax",%progbits
	.align	1
	.global	SEGGER_RTT_WriteNoLock
	.syntax unified
	.thumb
	.thumb_func
	.type	SEGGER_RTT_WriteNoLock, %function
SEGGER_RTT_WriteNoLock:
.LVL92:
.LFB215:
	.loc 1 895 95 is_stmt 1 view -0
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	.loc 1 896 3 view .LVU353
	.loc 1 897 3 view .LVU354
	.loc 1 898 3 view .LVU355
	.loc 1 899 3 view .LVU356
	.loc 1 901 3 view .LVU357
	.loc 1 905 3 view .LVU358
	.loc 1 895 95 is_stmt 0 view .LVU359
	push	{r3, r4, r5, r6, r7, lr}
.LCFI10:
	.loc 1 905 9 view .LVU360
	ldr	r3, .L70
	movs	r7, #24
	mla	r5, r0, r7, r7
	add	r5, r5, r3
.LVL93:
	.loc 1 909 3 is_stmt 1 view .LVU361
	.loc 1 909 16 is_stmt 0 view .LVU362
	mla	r3, r7, r0, r3
	.loc 1 895 95 view .LVU363
	mov	r6, r1
	.loc 1 909 16 view .LVU364
	ldr	r3, [r3, #44]
	.loc 1 909 3 view .LVU365
	cmp	r3, #1
	.loc 1 895 95 view .LVU366
	mov	r4, r2
	.loc 1 909 3 view .LVU367
	beq	.L64
	cmp	r3, #2
	beq	.L65
	cbnz	r3, .L66
	.loc 1 915 5 is_stmt 1 view .LVU368
	.loc 1 915 13 is_stmt 0 view .LVU369
	mov	r0, r5
.LVL94:
	.loc 1 915 13 view .LVU370
	bl	_GetAvailWriteSpace
.LVL95:
	.loc 1 916 5 is_stmt 1 view .LVU371
	.loc 1 916 8 is_stmt 0 view .LVU372
	cmp	r4, r0
	bls	.L69
.LVL96:
.L66:
	.loc 1 938 12 view .LVU373
	movs	r4, #0
.LVL97:
.L68:
	.loc 1 944 3 is_stmt 1 view .LVU374
	.loc 1 945 1 is_stmt 0 view .LVU375
	mov	r0, r4
	pop	{r3, r4, r5, r6, r7, pc}
.LVL98:
.L64:
	.loc 1 927 5 is_stmt 1 view .LVU376
	.loc 1 927 13 is_stmt 0 view .LVU377
	mov	r0, r5
.LVL99:
	.loc 1 927 13 view .LVU378
	bl	_GetAvailWriteSpace
.LVL100:
	.loc 1 928 5 is_stmt 1 view .LVU379
	.loc 1 928 12 is_stmt 0 view .LVU380
	cmp	r4, r0
	it	cs
	movcs	r4, r0
.LVL101:
.L69:
	.loc 1 929 5 is_stmt 1 view .LVU381
	mov	r2, r4
	mov	r1, r6
	mov	r0, r5
.LVL102:
	.loc 1 929 5 is_stmt 0 view .LVU382
	bl	_WriteNoCheck
.LVL103:
	.loc 1 930 5 is_stmt 1 view .LVU383
	b	.L68
.LVL104:
.L65:
	.loc 1 935 5 view .LVU384
	.loc 1 935 14 is_stmt 0 view .LVU385
	mov	r0, r5
.LVL105:
	.loc 1 945 1 view .LVU386
	pop	{r3, r4, r5, r6, r7, lr}
.LCFI11:
.LVL106:
	.loc 1 935 14 view .LVU387
	b	_WriteBlocking
.LVL107:
.L71:
	.loc 1 935 14 view .LVU388
	.align	2
.L70:
	.word	_SEGGER_RTT
.LFE215:
	.size	SEGGER_RTT_WriteNoLock, .-SEGGER_RTT_WriteNoLock
	.section	.text.SEGGER_RTT_Write,"ax",%progbits
	.align	1
	.global	SEGGER_RTT_Write
	.syntax unified
	.thumb
	.thumb_func
	.type	SEGGER_RTT_Write, %function
SEGGER_RTT_Write:
.LVL108:
.LFB216:
	.loc 1 966 89 is_stmt 1 view -0
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	.loc 1 967 3 view .LVU390
	.loc 1 969 3 view .LVU391
	.loc 1 969 3 view .LVU392
	.loc 1 966 89 is_stmt 0 view .LVU393
	push	{r0, r1, r4, r5, r6, lr}
.LCFI12:
	.loc 1 969 3 view .LVU394
	ldr	r3, .L74
	ldrb	r3, [r3]	@ zero_extendqisi2
	.loc 1 966 89 view .LVU395
	mov	r4, r0
	mov	r5, r1
	mov	r6, r2
	.loc 1 969 3 view .LVU396
	cbnz	r3, .L73
	.loc 1 969 3 is_stmt 1 discriminator 1 view .LVU397
	bl	_DoInit
.LVL109:
.L73:
	.loc 1 969 3 discriminator 3 view .LVU398
.LBB5:
	.loc 1 970 3 discriminator 3 view .LVU399
	movs	r3, #0
	add	r0, sp, #7
	strb	r3, [sp, #7]
	.loc 1 970 3 discriminator 3 view .LVU400
	bl	app_util_critical_region_enter
.LVL110:
	.loc 1 970 20 discriminator 3 view .LVU401
	.loc 1 974 3 discriminator 3 view .LVU402
	.loc 1 974 12 is_stmt 0 discriminator 3 view .LVU403
	mov	r2, r6
	mov	r1, r5
	mov	r0, r4
	bl	SEGGER_RTT_WriteNoLock
.LVL111:
	mov	r4, r0
.LVL112:
	.loc 1 978 3 is_stmt 1 discriminator 3 view .LVU404
	ldrb	r0, [sp, #7]	@ zero_extendqisi2
.LVL113:
	.loc 1 978 3 is_stmt 0 discriminator 3 view .LVU405
	bl	app_util_critical_region_exit
.LVL114:
.LBE5:
	.loc 1 978 22 is_stmt 1 discriminator 3 view .LVU406
	.loc 1 980 3 discriminator 3 view .LVU407
	.loc 1 981 1 is_stmt 0 discriminator 3 view .LVU408
	mov	r0, r4
	add	sp, sp, #8
.LCFI13:
	@ sp needed
	pop	{r4, r5, r6, pc}
.LVL115:
.L75:
	.loc 1 981 1 discriminator 3 view .LVU409
	.align	2
.L74:
	.word	_SEGGER_RTT
.LFE216:
	.size	SEGGER_RTT_Write, .-SEGGER_RTT_Write
	.section	.text.SEGGER_RTT_WriteString,"ax",%progbits
	.align	1
	.global	SEGGER_RTT_WriteString
	.syntax unified
	.thumb
	.thumb_func
	.type	SEGGER_RTT_WriteString, %function
SEGGER_RTT_WriteString:
.LVL116:
.LFB217:
	.loc 1 1003 70 is_stmt 1 view -0
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	.loc 1 1004 3 view .LVU411
	.loc 1 1006 3 view .LVU412
	.loc 1 1003 70 is_stmt 0 view .LVU413
	push	{r3, r4, r5, lr}
.LCFI14:
	.loc 1 1003 70 view .LVU414
	mov	r5, r0
	.loc 1 1006 9 view .LVU415
	mov	r0, r1
.LVL117:
	.loc 1 1003 70 view .LVU416
	mov	r4, r1
	.loc 1 1006 9 view .LVU417
	bl	strlen
.LVL118:
	.loc 1 1007 10 view .LVU418
	mov	r1, r4
	.loc 1 1006 9 view .LVU419
	mov	r2, r0
.LVL119:
	.loc 1 1007 3 is_stmt 1 view .LVU420
	.loc 1 1007 10 is_stmt 0 view .LVU421
	mov	r0, r5
.LVL120:
	.loc 1 1008 1 view .LVU422
	pop	{r3, r4, r5, lr}
.LCFI15:
.LVL121:
	.loc 1 1007 10 view .LVU423
	b	SEGGER_RTT_Write
.LVL122:
	.loc 1 1007 10 view .LVU424
.LFE217:
	.size	SEGGER_RTT_WriteString, .-SEGGER_RTT_WriteString
	.section	.text.SEGGER_RTT_PutCharSkipNoLock,"ax",%progbits
	.align	1
	.global	SEGGER_RTT_PutCharSkipNoLock
	.syntax unified
	.thumb
	.thumb_func
	.type	SEGGER_RTT_PutCharSkipNoLock, %function
SEGGER_RTT_PutCharSkipNoLock:
.LVL123:
.LFB218:
	.loc 1 1033 69 is_stmt 1 view -0
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	.loc 1 1034 3 view .LVU426
	.loc 1 1035 3 view .LVU427
	.loc 1 1036 3 view .LVU428
	.loc 1 1040 3 view .LVU429
	.loc 1 1044 3 view .LVU430
	.loc 1 1033 69 is_stmt 0 view .LVU431
	push	{r4, r5, r6, r7, lr}
.LCFI16:
	.loc 1 1044 16 view .LVU432
	ldr	r5, .L81
	movs	r2, #24
	mla	r4, r2, r0, r5
	ldr	r6, [r4, #36]
	.loc 1 1045 6 view .LVU433
	ldr	r7, [r4, #32]
	.loc 1 1044 9 view .LVU434
	adds	r3, r6, #1
.LVL124:
	.loc 1 1045 3 is_stmt 1 view .LVU435
	.loc 1 1046 11 is_stmt 0 view .LVU436
	cmp	r7, r3
	it	eq
	moveq	r3, #0
.LVL125:
	.loc 1 1051 3 is_stmt 1 view .LVU437
	.loc 1 1051 21 is_stmt 0 view .LVU438
	ldr	r7, [r4, #40]
	.loc 1 1051 6 view .LVU439
	cmp	r7, r3
	.loc 1 1052 5 is_stmt 1 view .LVU440
	.loc 1 1052 10 is_stmt 0 view .LVU441
	itttt	ne
	mlane	r2, r0, r2, r2
	addne	r5, r5, r2
	.loc 1 1054 12 view .LVU442
	movne	r0, #1
.LVL126:
	.loc 1 1052 34 view .LVU443
	ldrne	r2, [r5, #4]
	itet	ne
	strbne	r1, [r2, r6]
	.loc 1 1053 5 is_stmt 1 view .LVU444
	.loc 1 1056 12 is_stmt 0 view .LVU445
	moveq	r0, #0
	.loc 1 1053 18 view .LVU446
	strne	r3, [r4, #36]
	.loc 1 1054 5 is_stmt 1 view .LVU447
.LVL127:
	.loc 1 1059 3 view .LVU448
	.loc 1 1060 1 is_stmt 0 view .LVU449
	pop	{r4, r5, r6, r7, pc}
.L82:
	.align	2
.L81:
	.word	_SEGGER_RTT
.LFE218:
	.size	SEGGER_RTT_PutCharSkipNoLock, .-SEGGER_RTT_PutCharSkipNoLock
	.section	.text.SEGGER_RTT_PutCharSkip,"ax",%progbits
	.align	1
	.global	SEGGER_RTT_PutCharSkip
	.syntax unified
	.thumb
	.thumb_func
	.type	SEGGER_RTT_PutCharSkip, %function
SEGGER_RTT_PutCharSkip:
.LVL128:
.LFB219:
	.loc 1 1080 63 is_stmt 1 view -0
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	.loc 1 1081 3 view .LVU451
	.loc 1 1082 3 view .LVU452
	.loc 1 1083 3 view .LVU453
	.loc 1 1087 3 view .LVU454
	.loc 1 1087 3 view .LVU455
	.loc 1 1080 63 is_stmt 0 view .LVU456
	push	{r0, r1, r4, r5, r6, r7, r8, lr}
.LCFI17:
	.loc 1 1087 3 view .LVU457
	ldr	r5, .L88
	ldrb	r3, [r5]	@ zero_extendqisi2
	.loc 1 1080 63 view .LVU458
	mov	r8, r0
	mov	r7, r1
	.loc 1 1087 3 view .LVU459
	cbnz	r3, .L84
	.loc 1 1087 3 is_stmt 1 discriminator 1 view .LVU460
	bl	_DoInit
.LVL129:
.L84:
	.loc 1 1087 3 discriminator 3 view .LVU461
.LBB6:
	.loc 1 1088 3 discriminator 3 view .LVU462
	movs	r4, #0
	add	r0, sp, #7
	strb	r4, [sp, #7]
	.loc 1 1088 3 discriminator 3 view .LVU463
	bl	app_util_critical_region_enter
.LVL130:
	.loc 1 1088 20 discriminator 3 view .LVU464
	.loc 1 1092 3 discriminator 3 view .LVU465
	.loc 1 1096 3 discriminator 3 view .LVU466
	.loc 1 1096 16 is_stmt 0 discriminator 3 view .LVU467
	movs	r2, #24
	mla	r6, r2, r8, r5
	ldr	r0, [r6, #36]
	.loc 1 1097 6 discriminator 3 view .LVU468
	ldr	r1, [r6, #32]
	.loc 1 1096 9 discriminator 3 view .LVU469
	adds	r3, r0, #1
.LVL131:
	.loc 1 1097 3 is_stmt 1 discriminator 3 view .LVU470
	.loc 1 1098 11 is_stmt 0 discriminator 3 view .LVU471
	cmp	r1, r3
	it	eq
	moveq	r3, r4
.LVL132:
	.loc 1 1103 3 is_stmt 1 discriminator 3 view .LVU472
	.loc 1 1103 21 is_stmt 0 discriminator 3 view .LVU473
	ldr	r1, [r6, #40]
	.loc 1 1103 6 discriminator 3 view .LVU474
	cmp	r1, r3
	beq	.L86
	.loc 1 1104 5 is_stmt 1 view .LVU475
	.loc 1 1104 10 is_stmt 0 view .LVU476
	mla	r2, r8, r2, r2
	add	r5, r5, r2
	.loc 1 1106 12 view .LVU477
	movs	r4, #1
	.loc 1 1104 34 view .LVU478
	ldr	r2, [r5, #4]
	strb	r7, [r2, r0]
	.loc 1 1105 5 is_stmt 1 view .LVU479
	.loc 1 1105 18 is_stmt 0 view .LVU480
	str	r3, [r6, #36]
	.loc 1 1106 5 is_stmt 1 view .LVU481
.LVL133:
.L86:
	.loc 1 1113 3 view .LVU482
	ldrb	r0, [sp, #7]	@ zero_extendqisi2
	bl	app_util_critical_region_exit
.LVL134:
	.loc 1 1113 3 is_stmt 0 view .LVU483
.LBE6:
	.loc 1 1113 22 is_stmt 1 view .LVU484
	.loc 1 1115 3 view .LVU485
	.loc 1 1116 1 is_stmt 0 view .LVU486
	mov	r0, r4
	add	sp, sp, #8
.LCFI18:
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.LVL135:
.L89:
	.loc 1 1116 1 view .LVU487
	.align	2
.L88:
	.word	_SEGGER_RTT
.LFE219:
	.size	SEGGER_RTT_PutCharSkip, .-SEGGER_RTT_PutCharSkip
	.section	.text.SEGGER_RTT_PutChar,"ax",%progbits
	.align	1
	.global	SEGGER_RTT_PutChar
	.syntax unified
	.thumb
	.thumb_func
	.type	SEGGER_RTT_PutChar, %function
SEGGER_RTT_PutChar:
.LVL136:
.LFB220:
	.loc 1 1136 59 is_stmt 1 view -0
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	.loc 1 1137 3 view .LVU489
	.loc 1 1138 3 view .LVU490
	.loc 1 1139 3 view .LVU491
	.loc 1 1143 3 view .LVU492
	.loc 1 1143 3 view .LVU493
	.loc 1 1136 59 is_stmt 0 view .LVU494
	push	{r0, r1, r2, r4, r5, r6, r7, lr}
.LCFI19:
	.loc 1 1143 3 view .LVU495
	ldr	r4, .L99
	ldrb	r3, [r4]	@ zero_extendqisi2
	.loc 1 1136 59 view .LVU496
	mov	r6, r0
	mov	r5, r1
	.loc 1 1143 3 view .LVU497
	cbnz	r3, .L91
	.loc 1 1143 3 is_stmt 1 discriminator 1 view .LVU498
	bl	_DoInit
.LVL137:
.L91:
	.loc 1 1143 3 discriminator 3 view .LVU499
.LBB7:
	.loc 1 1144 3 discriminator 3 view .LVU500
	movs	r3, #0
	add	r0, sp, #7
	strb	r3, [sp, #7]
	.loc 1 1144 3 discriminator 3 view .LVU501
	bl	app_util_critical_region_enter
.LVL138:
	.loc 1 1144 20 discriminator 3 view .LVU502
	.loc 1 1148 3 discriminator 3 view .LVU503
	.loc 1 1152 3 discriminator 3 view .LVU504
	.loc 1 1152 16 is_stmt 0 discriminator 3 view .LVU505
	movs	r2, #24
	mla	r2, r2, r6, r4
	ldr	r0, [r2, #36]
	.loc 1 1153 6 discriminator 3 view .LVU506
	ldr	r1, [r2, #32]
	.loc 1 1152 9 discriminator 3 view .LVU507
	adds	r3, r0, #1
.LVL139:
	.loc 1 1153 3 is_stmt 1 discriminator 3 view .LVU508
	.loc 1 1154 11 is_stmt 0 discriminator 3 view .LVU509
	cmp	r1, r3
	it	eq
	moveq	r3, #0
.LVL140:
	.loc 1 1159 3 is_stmt 1 discriminator 3 view .LVU510
	.loc 1 1159 6 is_stmt 0 discriminator 3 view .LVU511
	ldr	r1, [r2, #44]
	cmp	r1, #2
.L98:
	bne	.L93
	.loc 1 1161 7 is_stmt 1 discriminator 1 view .LVU512
	.loc 1 1160 18 discriminator 1 view .LVU513
	.loc 1 1160 26 is_stmt 0 discriminator 1 view .LVU514
	ldr	r1, [r2, #40]
	.loc 1 1160 18 discriminator 1 view .LVU515
	cmp	r1, r3
	b	.L98
.L93:
	.loc 1 1167 3 is_stmt 1 view .LVU516
	.loc 1 1167 21 is_stmt 0 view .LVU517
	movs	r2, #24
	mla	r1, r2, r6, r4
	ldr	r7, [r1, #40]
	.loc 1 1167 6 view .LVU518
	cmp	r7, r3
	.loc 1 1168 5 is_stmt 1 view .LVU519
	.loc 1 1168 10 is_stmt 0 view .LVU520
	ittet	ne
	mlane	r2, r6, r2, r2
	addne	r4, r4, r2
	.loc 1 1172 12 view .LVU521
	moveq	r4, #0
	.loc 1 1168 34 view .LVU522
	ldrne	r2, [r4, #4]
	it	ne
	strbne	r5, [r2, r0]
	.loc 1 1169 5 is_stmt 1 view .LVU523
	.loc 1 1177 3 is_stmt 0 view .LVU524
	ldrb	r0, [sp, #7]	@ zero_extendqisi2
	.loc 1 1169 18 view .LVU525
	itt	ne
	strne	r3, [r1, #36]
	.loc 1 1170 5 is_stmt 1 view .LVU526
.LVL141:
	.loc 1 1170 12 is_stmt 0 view .LVU527
	movne	r4, #1
.LVL142:
	.loc 1 1177 3 is_stmt 1 view .LVU528
	bl	app_util_critical_region_exit
.LVL143:
	.loc 1 1177 3 is_stmt 0 view .LVU529
.LBE7:
	.loc 1 1177 22 is_stmt 1 view .LVU530
	.loc 1 1179 3 view .LVU531
	.loc 1 1180 1 is_stmt 0 view .LVU532
	mov	r0, r4
	add	sp, sp, #12
.LCFI20:
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.LVL144:
.L100:
	.loc 1 1180 1 view .LVU533
	.align	2
.L99:
	.word	_SEGGER_RTT
.LFE220:
	.size	SEGGER_RTT_PutChar, .-SEGGER_RTT_PutChar
	.section	.text.SEGGER_RTT_GetKey,"ax",%progbits
	.align	1
	.global	SEGGER_RTT_GetKey
	.syntax unified
	.thumb
	.thumb_func
	.type	SEGGER_RTT_GetKey, %function
SEGGER_RTT_GetKey:
.LFB221:
	.loc 1 1197 29 is_stmt 1 view -0
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	.loc 1 1198 3 view .LVU535
	.loc 1 1199 3 view .LVU536
	.loc 1 1201 3 view .LVU537
	.loc 1 1197 29 is_stmt 0 view .LVU538
	push	{r0, r1, r2, lr}
.LCFI21:
	.loc 1 1201 12 view .LVU539
	movs	r2, #1
	add	r1, sp, #7
	movs	r0, #0
	bl	SEGGER_RTT_Read
.LVL145:
	.loc 1 1202 3 is_stmt 1 view .LVU540
	.loc 1 1202 6 is_stmt 0 view .LVU541
	cmp	r0, #1
	.loc 1 1203 5 is_stmt 1 view .LVU542
	.loc 1 1203 7 is_stmt 0 view .LVU543
	ite	eq
	ldrbeq	r0, [sp, #7]	@ zero_extendqisi2
.LVL146:
	.loc 1 1205 7 view .LVU544
	movne	r0, #-1
.LVL147:
	.loc 1 1207 3 is_stmt 1 view .LVU545
	.loc 1 1208 1 is_stmt 0 view .LVU546
	add	sp, sp, #12
.LCFI22:
	@ sp needed
	ldr	pc, [sp], #4
.LFE221:
	.size	SEGGER_RTT_GetKey, .-SEGGER_RTT_GetKey
	.section	.text.SEGGER_RTT_WaitKey,"ax",%progbits
	.align	1
	.global	SEGGER_RTT_WaitKey
	.syntax unified
	.thumb
	.thumb_func
	.type	SEGGER_RTT_WaitKey, %function
SEGGER_RTT_WaitKey:
.LFB222:
	.loc 1 1225 30 is_stmt 1 view -0
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
.LCFI23:
.L105:
	.loc 1 1226 3 discriminator 1 view .LVU548
	.loc 1 1228 3 discriminator 1 view .LVU549
	.loc 1 1229 5 discriminator 1 view .LVU550
	.loc 1 1229 9 is_stmt 0 discriminator 1 view .LVU551
	bl	SEGGER_RTT_GetKey
.LVL148:
	.loc 1 1230 14 is_stmt 1 discriminator 1 view .LVU552
	cmp	r0, #0
	blt	.L105
	.loc 1 1231 3 view .LVU553
	.loc 1 1232 1 is_stmt 0 view .LVU554
	pop	{r3, pc}
.LFE222:
	.size	SEGGER_RTT_WaitKey, .-SEGGER_RTT_WaitKey
	.section	.text.SEGGER_RTT_HasKey,"ax",%progbits
	.align	1
	.global	SEGGER_RTT_HasKey
	.syntax unified
	.thumb
	.thumb_func
	.type	SEGGER_RTT_HasKey, %function
SEGGER_RTT_HasKey:
.LFB223:
	.loc 1 1248 29 is_stmt 1 view -0
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	.loc 1 1249 3 view .LVU556
	.loc 1 1250 3 view .LVU557
	.loc 1 1252 3 view .LVU558
	.loc 1 1252 3 view .LVU559
	.loc 1 1248 29 is_stmt 0 view .LVU560
	push	{r4, lr}
.LCFI24:
	.loc 1 1252 3 view .LVU561
	ldr	r4, .L109
	ldrb	r3, [r4]	@ zero_extendqisi2
	cbnz	r3, .L108
	.loc 1 1252 3 is_stmt 1 discriminator 1 view .LVU562
	bl	_DoInit
.LVL149:
.L108:
	.loc 1 1252 3 discriminator 3 view .LVU563
	.loc 1 1253 3 discriminator 3 view .LVU564
	.loc 1 1254 3 discriminator 3 view .LVU565
	.loc 1 1254 36 is_stmt 0 discriminator 3 view .LVU566
	ldr	r3, [r4, #84]
.LVL150:
	.loc 1 1259 3 is_stmt 1 discriminator 3 view .LVU567
	.loc 1 1254 6 is_stmt 0 discriminator 3 view .LVU568
	ldr	r0, [r4, #88]
	.loc 1 1260 1 discriminator 3 view .LVU569
	subs	r0, r0, r3
	it	ne
	movne	r0, #1
	pop	{r4, pc}
.L110:
	.align	2
.L109:
	.word	_SEGGER_RTT
.LFE223:
	.size	SEGGER_RTT_HasKey, .-SEGGER_RTT_HasKey
	.section	.text.SEGGER_RTT_HasData,"ax",%progbits
	.align	1
	.global	SEGGER_RTT_HasData
	.syntax unified
	.thumb
	.thumb_func
	.type	SEGGER_RTT_HasData, %function
SEGGER_RTT_HasData:
.LVL151:
.LFB224:
	.loc 1 1274 51 is_stmt 1 view -0
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	.loc 1 1275 3 view .LVU571
	.loc 1 1276 3 view .LVU572
	.loc 1 1278 3 view .LVU573
	.loc 1 1279 3 view .LVU574
	.loc 1 1279 5 is_stmt 0 view .LVU575
	ldr	r3, .L112
	movs	r2, #24
	mla	r3, r2, r0, r3
	ldr	r0, [r3, #84]
.LVL152:
	.loc 1 1280 3 is_stmt 1 view .LVU576
	.loc 1 1280 12 is_stmt 0 view .LVU577
	ldr	r3, [r3, #88]
	.loc 1 1281 1 view .LVU578
	subs	r0, r0, r3
.LVL153:
	.loc 1 1281 1 view .LVU579
	bx	lr
.L113:
	.align	2
.L112:
	.word	_SEGGER_RTT
.LFE224:
	.size	SEGGER_RTT_HasData, .-SEGGER_RTT_HasData
	.section	.text.SEGGER_RTT_AllocDownBuffer,"ax",%progbits
	.align	1
	.global	SEGGER_RTT_AllocDownBuffer
	.syntax unified
	.thumb
	.thumb_func
	.type	SEGGER_RTT_AllocDownBuffer, %function
SEGGER_RTT_AllocDownBuffer:
.LVL154:
.LFB225:
	.loc 1 1302 103 is_stmt 1 view -0
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	.loc 1 1303 3 view .LVU581
	.loc 1 1305 3 view .LVU582
	.loc 1 1305 3 view .LVU583
	.loc 1 1302 103 is_stmt 0 view .LVU584
	push	{r0, r1, r2, r4, r5, r6, r7, r8, r9, lr}
.LCFI25:
	.loc 1 1305 3 view .LVU585
	ldr	r4, .L122
	.loc 1 1302 103 view .LVU586
	mov	r6, r3
	.loc 1 1305 3 view .LVU587
	ldrb	r3, [r4]	@ zero_extendqisi2
.LVL155:
	.loc 1 1302 103 view .LVU588
	mov	r9, r0
	mov	r8, r1
	mov	r7, r2
	.loc 1 1305 3 view .LVU589
	cbnz	r3, .L115
	.loc 1 1305 3 is_stmt 1 discriminator 1 view .LVU590
	bl	_DoInit
.LVL156:
.L115:
	.loc 1 1305 3 discriminator 3 view .LVU591
.LBB8:
	.loc 1 1306 3 discriminator 3 view .LVU592
	movs	r3, #0
	add	r0, sp, #7
	strb	r3, [sp, #7]
	.loc 1 1306 3 discriminator 3 view .LVU593
	bl	app_util_critical_region_enter
.LVL157:
	.loc 1 1306 20 discriminator 3 view .LVU594
	.loc 1 1307 3 discriminator 3 view .LVU595
	.loc 1 1308 3 discriminator 3 view .LVU596
	.loc 1 1309 5 discriminator 3 view .LVU597
	.loc 1 1309 8 is_stmt 0 discriminator 3 view .LVU598
	ldr	r5, [r4, #76]
	cbz	r5, .L116
	.loc 1 1312 5 is_stmt 1 view .LVU599
.LVL158:
	.loc 1 1313 24 view .LVU600
	ldr	r3, [r4, #20]
	cmp	r3, #1
	ble	.L119
	.loc 1 1308 3 view .LVU601
	.loc 1 1309 5 view .LVU602
	.loc 1 1309 8 is_stmt 0 view .LVU603
	ldr	r3, [r4, #100]
	.loc 1 1312 16 view .LVU604
	cmp	r3, #0
	ite	ne
	movne	r5, #2
	moveq	r5, #1
.LVL159:
.L116:
	.loc 1 1314 3 is_stmt 1 view .LVU605
	.loc 1 1314 6 is_stmt 0 view .LVU606
	ldr	r3, [r4, #20]
	cmp	r3, r5
	ble	.L121
	.loc 1 1315 5 is_stmt 1 view .LVU607
	.loc 1 1315 49 is_stmt 0 view .LVU608
	movs	r3, #24
	mla	r4, r3, r5, r4
	.loc 1 1316 49 view .LVU609
	strd	r9, r8, [r4, #72]
	.loc 1 1317 5 is_stmt 1 view .LVU610
	.loc 1 1318 49 is_stmt 0 view .LVU611
	movs	r3, #0
	.loc 1 1317 49 view .LVU612
	str	r7, [r4, #80]
	.loc 1 1318 5 is_stmt 1 view .LVU613
	.loc 1 1318 49 is_stmt 0 view .LVU614
	str	r3, [r4, #88]
	.loc 1 1319 5 is_stmt 1 view .LVU615
	.loc 1 1319 49 is_stmt 0 view .LVU616
	str	r3, [r4, #84]
	.loc 1 1320 5 is_stmt 1 view .LVU617
	.loc 1 1320 49 is_stmt 0 view .LVU618
	str	r6, [r4, #92]
.LVL160:
.L117:
	.loc 1 1324 3 is_stmt 1 view .LVU619
	ldrb	r0, [sp, #7]	@ zero_extendqisi2
	bl	app_util_critical_region_exit
.LVL161:
.LBE8:
	.loc 1 1324 22 view .LVU620
	.loc 1 1325 3 view .LVU621
	.loc 1 1326 1 is_stmt 0 view .LVU622
	mov	r0, r5
	add	sp, sp, #12
.LCFI26:
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, pc}
.LVL162:
.L119:
.LCFI27:
.LBB9:
	.loc 1 1312 16 view .LVU623
	movs	r5, #1
	b	.L116
.LVL163:
.L121:
	.loc 1 1322 17 view .LVU624
	mov	r5, #-1
.LVL164:
	.loc 1 1322 17 view .LVU625
	b	.L117
.L123:
	.align	2
.L122:
	.word	_SEGGER_RTT
.LBE9:
.LFE225:
	.size	SEGGER_RTT_AllocDownBuffer, .-SEGGER_RTT_AllocDownBuffer
	.section	.text.SEGGER_RTT_AllocUpBuffer,"ax",%progbits
	.align	1
	.global	SEGGER_RTT_AllocUpBuffer
	.syntax unified
	.thumb
	.thumb_func
	.type	SEGGER_RTT_AllocUpBuffer, %function
SEGGER_RTT_AllocUpBuffer:
.LVL165:
.LFB226:
	.loc 1 1347 101 is_stmt 1 view -0
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	.loc 1 1348 3 view .LVU627
	.loc 1 1350 3 view .LVU628
	.loc 1 1350 3 view .LVU629
	.loc 1 1347 101 is_stmt 0 view .LVU630
	push	{r0, r1, r2, r4, r5, r6, r7, r8, r9, lr}
.LCFI28:
	.loc 1 1350 3 view .LVU631
	ldr	r4, .L132
	.loc 1 1347 101 view .LVU632
	mov	r6, r3
	.loc 1 1350 3 view .LVU633
	ldrb	r3, [r4]	@ zero_extendqisi2
.LVL166:
	.loc 1 1347 101 view .LVU634
	mov	r9, r0
	mov	r8, r1
	mov	r7, r2
	.loc 1 1350 3 view .LVU635
	cbnz	r3, .L125
	.loc 1 1350 3 is_stmt 1 discriminator 1 view .LVU636
	bl	_DoInit
.LVL167:
.L125:
	.loc 1 1350 3 discriminator 3 view .LVU637
.LBB10:
	.loc 1 1351 3 discriminator 3 view .LVU638
	movs	r3, #0
	add	r0, sp, #7
	strb	r3, [sp, #7]
	.loc 1 1351 3 discriminator 3 view .LVU639
	bl	app_util_critical_region_enter
.LVL168:
	.loc 1 1351 20 discriminator 3 view .LVU640
	.loc 1 1352 3 discriminator 3 view .LVU641
	.loc 1 1353 3 discriminator 3 view .LVU642
	.loc 1 1354 5 discriminator 3 view .LVU643
	.loc 1 1354 8 is_stmt 0 discriminator 3 view .LVU644
	ldr	r5, [r4, #28]
	cbz	r5, .L126
	.loc 1 1357 5 is_stmt 1 view .LVU645
.LVL169:
	.loc 1 1358 24 view .LVU646
	ldr	r3, [r4, #16]
	cmp	r3, #1
	ble	.L129
	.loc 1 1353 3 view .LVU647
	.loc 1 1354 5 view .LVU648
	.loc 1 1354 8 is_stmt 0 view .LVU649
	ldr	r3, [r4, #52]
	.loc 1 1357 16 view .LVU650
	cmp	r3, #0
	ite	ne
	movne	r5, #2
	moveq	r5, #1
.LVL170:
.L126:
	.loc 1 1359 3 is_stmt 1 view .LVU651
	.loc 1 1359 6 is_stmt 0 view .LVU652
	ldr	r3, [r4, #16]
	cmp	r3, r5
	ble	.L131
	.loc 1 1360 5 is_stmt 1 view .LVU653
	.loc 1 1360 47 is_stmt 0 view .LVU654
	movs	r3, #24
	mla	r2, r5, r3, r3
	adds	r1, r4, r2
	str	r9, [r4, r2]
	.loc 1 1361 5 is_stmt 1 view .LVU655
	.loc 1 1362 47 is_stmt 0 view .LVU656
	mla	r4, r3, r5, r4
	.loc 1 1363 47 view .LVU657
	movs	r3, #0
	.loc 1 1361 47 view .LVU658
	str	r8, [r1, #4]
	.loc 1 1362 5 is_stmt 1 view .LVU659
	.loc 1 1362 47 is_stmt 0 view .LVU660
	str	r7, [r4, #32]
	.loc 1 1363 5 is_stmt 1 view .LVU661
	.loc 1 1363 47 is_stmt 0 view .LVU662
	str	r3, [r4, #40]
	.loc 1 1364 5 is_stmt 1 view .LVU663
	.loc 1 1364 47 is_stmt 0 view .LVU664
	str	r3, [r4, #36]
	.loc 1 1365 5 is_stmt 1 view .LVU665
	.loc 1 1365 47 is_stmt 0 view .LVU666
	str	r6, [r4, #44]
.LVL171:
.L127:
	.loc 1 1369 3 is_stmt 1 view .LVU667
	ldrb	r0, [sp, #7]	@ zero_extendqisi2
	bl	app_util_critical_region_exit
.LVL172:
.LBE10:
	.loc 1 1369 22 view .LVU668
	.loc 1 1370 3 view .LVU669
	.loc 1 1371 1 is_stmt 0 view .LVU670
	mov	r0, r5
	add	sp, sp, #12
.LCFI29:
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, pc}
.LVL173:
.L129:
.LCFI30:
.LBB11:
	.loc 1 1357 16 view .LVU671
	movs	r5, #1
	b	.L126
.LVL174:
.L131:
	.loc 1 1367 17 view .LVU672
	mov	r5, #-1
.LVL175:
	.loc 1 1367 17 view .LVU673
	b	.L127
.L133:
	.align	2
.L132:
	.word	_SEGGER_RTT
.LBE11:
.LFE226:
	.size	SEGGER_RTT_AllocUpBuffer, .-SEGGER_RTT_AllocUpBuffer
	.section	.text.SEGGER_RTT_ConfigUpBuffer,"ax",%progbits
	.align	1
	.global	SEGGER_RTT_ConfigUpBuffer
	.syntax unified
	.thumb
	.thumb_func
	.type	SEGGER_RTT_ConfigUpBuffer, %function
SEGGER_RTT_ConfigUpBuffer:
.LVL176:
.LFB227:
	.loc 1 1398 124 is_stmt 1 view -0
	@ args = 4, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	.loc 1 1399 3 view .LVU675
	.loc 1 1401 3 view .LVU676
	.loc 1 1401 3 view .LVU677
	.loc 1 1398 124 is_stmt 0 view .LVU678
	push	{r0, r1, r2, r4, r5, r6, r7, r8, r9, lr}
.LCFI31:
	.loc 1 1401 3 view .LVU679
	ldr	r4, .L142
	.loc 1 1398 124 view .LVU680
	mov	r7, r3
	.loc 1 1401 3 view .LVU681
	ldrb	r3, [r4]	@ zero_extendqisi2
.LVL177:
	.loc 1 1398 124 view .LVU682
	mov	r5, r0
	mov	r9, r1
	mov	r8, r2
	.loc 1 1401 3 view .LVU683
	cbnz	r3, .L135
	.loc 1 1401 3 is_stmt 1 discriminator 1 view .LVU684
	bl	_DoInit
.LVL178:
.L135:
	.loc 1 1401 3 discriminator 3 view .LVU685
	.loc 1 1402 3 discriminator 3 view .LVU686
	.loc 1 1402 6 is_stmt 0 discriminator 3 view .LVU687
	ldr	r3, [r4, #16]
	cmp	r3, r5
	bls	.L138
.LBB12:
	.loc 1 1403 5 is_stmt 1 view .LVU688
	movs	r6, #0
	add	r0, sp, #7
	strb	r6, [sp, #7]
	.loc 1 1403 5 view .LVU689
	bl	app_util_critical_region_enter
.LVL179:
	.loc 1 1403 22 view .LVU690
	.loc 1 1404 5 view .LVU691
	.loc 1 1404 8 is_stmt 0 view .LVU692
	cbz	r5, .L137
	.loc 1 1405 7 is_stmt 1 view .LVU693
	.loc 1 1405 49 is_stmt 0 view .LVU694
	movs	r0, #24
	mla	r3, r5, r0, r0
	adds	r2, r4, r3
	str	r9, [r4, r3]
	.loc 1 1406 7 is_stmt 1 view .LVU695
	.loc 1 1406 49 is_stmt 0 view .LVU696
	str	r8, [r2, #4]
	.loc 1 1407 7 is_stmt 1 view .LVU697
	.loc 1 1407 49 is_stmt 0 view .LVU698
	mla	r2, r0, r5, r4
	str	r7, [r2, #32]
	.loc 1 1408 7 is_stmt 1 view .LVU699
	.loc 1 1408 49 is_stmt 0 view .LVU700
	str	r6, [r2, #40]
	.loc 1 1409 7 is_stmt 1 view .LVU701
	.loc 1 1409 49 is_stmt 0 view .LVU702
	str	r6, [r2, #36]
.L137:
	.loc 1 1411 5 is_stmt 1 view .LVU703
	.loc 1 1411 49 is_stmt 0 view .LVU704
	movs	r3, #24
	mla	r4, r3, r5, r4
	.loc 1 1412 5 view .LVU705
	ldrb	r0, [sp, #7]	@ zero_extendqisi2
	.loc 1 1411 49 view .LVU706
	ldr	r3, [sp, #40]
	str	r3, [r4, #44]
	.loc 1 1412 5 is_stmt 1 view .LVU707
	bl	app_util_critical_region_exit
.LVL180:
.LBE12:
	.loc 1 1412 24 view .LVU708
	.loc 1 1413 5 view .LVU709
	.loc 1 1413 7 is_stmt 0 view .LVU710
	movs	r0, #0
.LVL181:
.L134:
	.loc 1 1418 1 view .LVU711
	add	sp, sp, #12
.LCFI32:
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, pc}
.LVL182:
.L138:
.LCFI33:
	.loc 1 1415 7 view .LVU712
	mov	r0, #-1
.LVL183:
	.loc 1 1417 3 is_stmt 1 view .LVU713
	.loc 1 1417 10 is_stmt 0 view .LVU714
	b	.L134
.L143:
	.align	2
.L142:
	.word	_SEGGER_RTT
.LFE227:
	.size	SEGGER_RTT_ConfigUpBuffer, .-SEGGER_RTT_ConfigUpBuffer
	.section	.text.SEGGER_RTT_ConfigDownBuffer,"ax",%progbits
	.align	1
	.global	SEGGER_RTT_ConfigDownBuffer
	.syntax unified
	.thumb
	.thumb_func
	.type	SEGGER_RTT_ConfigDownBuffer, %function
SEGGER_RTT_ConfigDownBuffer:
.LVL184:
.LFB228:
	.loc 1 1445 126 is_stmt 1 view -0
	@ args = 4, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	.loc 1 1446 3 view .LVU716
	.loc 1 1448 3 view .LVU717
	.loc 1 1448 3 view .LVU718
	.loc 1 1445 126 is_stmt 0 view .LVU719
	push	{r0, r1, r4, r5, r6, r7, r8, r9, r10, lr}
.LCFI34:
	.loc 1 1448 3 view .LVU720
	ldr	r5, .L152
	.loc 1 1445 126 view .LVU721
	mov	r8, r3
	.loc 1 1448 3 view .LVU722
	ldrb	r3, [r5]	@ zero_extendqisi2
.LVL185:
	.loc 1 1445 126 view .LVU723
	mov	r6, r0
	mov	r10, r1
	mov	r9, r2
	.loc 1 1448 3 view .LVU724
	cbnz	r3, .L145
	.loc 1 1448 3 is_stmt 1 discriminator 1 view .LVU725
	bl	_DoInit
.LVL186:
.L145:
	.loc 1 1448 3 discriminator 3 view .LVU726
	.loc 1 1449 3 discriminator 3 view .LVU727
	.loc 1 1449 6 is_stmt 0 discriminator 3 view .LVU728
	ldr	r3, [r5, #20]
	cmp	r3, r6
	bls	.L148
.LBB13:
	.loc 1 1450 5 is_stmt 1 view .LVU729
	movs	r7, #0
	add	r0, sp, #7
	strb	r7, [sp, #7]
	.loc 1 1450 5 view .LVU730
	bl	app_util_critical_region_enter
.LVL187:
	.loc 1 1450 22 view .LVU731
	.loc 1 1451 5 view .LVU732
	.loc 1 1451 8 is_stmt 0 view .LVU733
	cbz	r6, .L147
	.loc 1 1452 7 is_stmt 1 view .LVU734
	.loc 1 1452 51 is_stmt 0 view .LVU735
	movs	r4, #24
	mla	r4, r4, r6, r5
	.loc 1 1453 51 view .LVU736
	strd	r10, r9, [r4, #72]
	.loc 1 1454 7 is_stmt 1 view .LVU737
	.loc 1 1454 51 is_stmt 0 view .LVU738
	str	r8, [r4, #80]
	.loc 1 1455 7 is_stmt 1 view .LVU739
	.loc 1 1455 51 is_stmt 0 view .LVU740
	str	r7, [r4, #88]
	.loc 1 1456 7 is_stmt 1 view .LVU741
	.loc 1 1456 51 is_stmt 0 view .LVU742
	str	r7, [r4, #84]
.L147:
	.loc 1 1458 5 is_stmt 1 view .LVU743
	.loc 1 1458 51 is_stmt 0 view .LVU744
	movs	r3, #24
	mla	r5, r3, r6, r5
	.loc 1 1459 5 view .LVU745
	ldrb	r0, [sp, #7]	@ zero_extendqisi2
	.loc 1 1458 51 view .LVU746
	ldr	r3, [sp, #40]
	str	r3, [r5, #92]
	.loc 1 1459 5 is_stmt 1 view .LVU747
	bl	app_util_critical_region_exit
.LVL188:
.LBE13:
	.loc 1 1459 24 view .LVU748
	.loc 1 1460 5 view .LVU749
	.loc 1 1460 7 is_stmt 0 view .LVU750
	movs	r0, #0
.LVL189:
.L144:
	.loc 1 1465 1 view .LVU751
	add	sp, sp, #8
.LCFI35:
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
.LVL190:
.L148:
.LCFI36:
	.loc 1 1462 7 view .LVU752
	mov	r0, #-1
.LVL191:
	.loc 1 1464 3 is_stmt 1 view .LVU753
	.loc 1 1464 10 is_stmt 0 view .LVU754
	b	.L144
.L153:
	.align	2
.L152:
	.word	_SEGGER_RTT
.LFE228:
	.size	SEGGER_RTT_ConfigDownBuffer, .-SEGGER_RTT_ConfigDownBuffer
	.section	.text.SEGGER_RTT_SetNameUpBuffer,"ax",%progbits
	.align	1
	.global	SEGGER_RTT_SetNameUpBuffer
	.syntax unified
	.thumb
	.thumb_func
	.type	SEGGER_RTT_SetNameUpBuffer, %function
SEGGER_RTT_SetNameUpBuffer:
.LVL192:
.LFB229:
	.loc 1 1483 73 is_stmt 1 view -0
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	.loc 1 1484 3 view .LVU756
	.loc 1 1486 3 view .LVU757
	.loc 1 1486 3 view .LVU758
	.loc 1 1483 73 is_stmt 0 view .LVU759
	push	{r0, r1, r2, r4, r5, r6, r7, lr}
.LCFI37:
	.loc 1 1486 3 view .LVU760
	ldr	r6, .L158
	ldrb	r3, [r6]	@ zero_extendqisi2
	.loc 1 1483 73 view .LVU761
	mov	r5, r0
	mov	r7, r1
	.loc 1 1486 3 view .LVU762
	cbnz	r3, .L155
	.loc 1 1486 3 is_stmt 1 discriminator 1 view .LVU763
	bl	_DoInit
.LVL193:
.L155:
	.loc 1 1486 3 discriminator 3 view .LVU764
	.loc 1 1487 3 discriminator 3 view .LVU765
	.loc 1 1487 6 is_stmt 0 discriminator 3 view .LVU766
	ldr	r3, [r6, #16]
	cmp	r3, r5
	bls	.L157
.LBB14:
	.loc 1 1488 5 is_stmt 1 view .LVU767
	movs	r4, #0
	add	r0, sp, #7
	strb	r4, [sp, #7]
	.loc 1 1488 5 view .LVU768
	bl	app_util_critical_region_enter
.LVL194:
	.loc 1 1488 22 view .LVU769
	.loc 1 1489 5 view .LVU770
	.loc 1 1489 40 is_stmt 0 view .LVU771
	movs	r3, #24
	mla	r3, r5, r3, r3
	.loc 1 1490 5 view .LVU772
	ldrb	r0, [sp, #7]	@ zero_extendqisi2
	.loc 1 1489 40 view .LVU773
	str	r7, [r6, r3]
	.loc 1 1490 5 is_stmt 1 view .LVU774
	bl	app_util_critical_region_exit
.LVL195:
.LBE14:
	.loc 1 1490 24 view .LVU775
	.loc 1 1491 5 view .LVU776
	.loc 1 1491 7 is_stmt 0 view .LVU777
	mov	r0, r4
.LVL196:
.L154:
	.loc 1 1496 1 view .LVU778
	add	sp, sp, #12
.LCFI38:
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.LVL197:
.L157:
.LCFI39:
	.loc 1 1493 7 view .LVU779
	mov	r0, #-1
.LVL198:
	.loc 1 1495 3 is_stmt 1 view .LVU780
	.loc 1 1495 10 is_stmt 0 view .LVU781
	b	.L154
.L159:
	.align	2
.L158:
	.word	_SEGGER_RTT
.LFE229:
	.size	SEGGER_RTT_SetNameUpBuffer, .-SEGGER_RTT_SetNameUpBuffer
	.section	.text.SEGGER_RTT_SetNameDownBuffer,"ax",%progbits
	.align	1
	.global	SEGGER_RTT_SetNameDownBuffer
	.syntax unified
	.thumb
	.thumb_func
	.type	SEGGER_RTT_SetNameDownBuffer, %function
SEGGER_RTT_SetNameDownBuffer:
.LVL199:
.LFB230:
	.loc 1 1514 75 is_stmt 1 view -0
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	.loc 1 1515 3 view .LVU783
	.loc 1 1517 3 view .LVU784
	.loc 1 1517 3 view .LVU785
	.loc 1 1514 75 is_stmt 0 view .LVU786
	push	{r0, r1, r2, r4, r5, r6, r7, lr}
.LCFI40:
	.loc 1 1517 3 view .LVU787
	ldr	r5, .L164
	ldrb	r3, [r5]	@ zero_extendqisi2
	.loc 1 1514 75 view .LVU788
	mov	r6, r0
	mov	r7, r1
	.loc 1 1517 3 view .LVU789
	cbnz	r3, .L161
	.loc 1 1517 3 is_stmt 1 discriminator 1 view .LVU790
	bl	_DoInit
.LVL200:
.L161:
	.loc 1 1517 3 discriminator 3 view .LVU791
	.loc 1 1518 3 discriminator 3 view .LVU792
	.loc 1 1518 6 is_stmt 0 discriminator 3 view .LVU793
	ldr	r3, [r5, #20]
	cmp	r3, r6
	bls	.L163
.LBB15:
	.loc 1 1519 5 is_stmt 1 view .LVU794
	movs	r4, #0
	add	r0, sp, #7
	strb	r4, [sp, #7]
	.loc 1 1519 5 view .LVU795
	bl	app_util_critical_region_enter
.LVL201:
	.loc 1 1519 22 view .LVU796
	.loc 1 1520 5 view .LVU797
	.loc 1 1520 42 is_stmt 0 view .LVU798
	movs	r3, #24
	mla	r5, r3, r6, r5
	.loc 1 1521 5 view .LVU799
	ldrb	r0, [sp, #7]	@ zero_extendqisi2
	.loc 1 1520 42 view .LVU800
	str	r7, [r5, #72]
	.loc 1 1521 5 is_stmt 1 view .LVU801
	bl	app_util_critical_region_exit
.LVL202:
.LBE15:
	.loc 1 1521 24 view .LVU802
	.loc 1 1522 5 view .LVU803
	.loc 1 1522 7 is_stmt 0 view .LVU804
	mov	r0, r4
.LVL203:
.L160:
	.loc 1 1527 1 view .LVU805
	add	sp, sp, #12
.LCFI41:
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.LVL204:
.L163:
.LCFI42:
	.loc 1 1524 7 view .LVU806
	mov	r0, #-1
.LVL205:
	.loc 1 1526 3 is_stmt 1 view .LVU807
	.loc 1 1526 10 is_stmt 0 view .LVU808
	b	.L160
.L165:
	.align	2
.L164:
	.word	_SEGGER_RTT
.LFE230:
	.size	SEGGER_RTT_SetNameDownBuffer, .-SEGGER_RTT_SetNameDownBuffer
	.section	.text.SEGGER_RTT_SetFlagsUpBuffer,"ax",%progbits
	.align	1
	.global	SEGGER_RTT_SetFlagsUpBuffer
	.syntax unified
	.thumb
	.thumb_func
	.type	SEGGER_RTT_SetFlagsUpBuffer, %function
SEGGER_RTT_SetFlagsUpBuffer:
.LVL206:
.LFB231:
	.loc 1 1545 71 is_stmt 1 view -0
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	.loc 1 1546 3 view .LVU810
	.loc 1 1548 3 view .LVU811
	.loc 1 1548 3 view .LVU812
	.loc 1 1545 71 is_stmt 0 view .LVU813
	push	{r0, r1, r2, r4, r5, r6, r7, lr}
.LCFI43:
	.loc 1 1548 3 view .LVU814
	ldr	r5, .L170
	ldrb	r3, [r5]	@ zero_extendqisi2
	.loc 1 1545 71 view .LVU815
	mov	r6, r0
	mov	r7, r1
	.loc 1 1548 3 view .LVU816
	cbnz	r3, .L167
	.loc 1 1548 3 is_stmt 1 discriminator 1 view .LVU817
	bl	_DoInit
.LVL207:
.L167:
	.loc 1 1548 3 discriminator 3 view .LVU818
	.loc 1 1549 3 discriminator 3 view .LVU819
	.loc 1 1549 6 is_stmt 0 discriminator 3 view .LVU820
	ldr	r3, [r5, #16]
	cmp	r3, r6
	bls	.L169
.LBB16:
	.loc 1 1550 5 is_stmt 1 view .LVU821
	movs	r4, #0
	add	r0, sp, #7
	strb	r4, [sp, #7]
	.loc 1 1550 5 view .LVU822
	bl	app_util_critical_region_enter
.LVL208:
	.loc 1 1550 22 view .LVU823
	.loc 1 1551 5 view .LVU824
	.loc 1 1551 40 is_stmt 0 view .LVU825
	movs	r3, #24
	mla	r5, r3, r6, r5
	.loc 1 1552 5 view .LVU826
	ldrb	r0, [sp, #7]	@ zero_extendqisi2
	.loc 1 1551 40 view .LVU827
	str	r7, [r5, #44]
	.loc 1 1552 5 is_stmt 1 view .LVU828
	bl	app_util_critical_region_exit
.LVL209:
.LBE16:
	.loc 1 1552 24 view .LVU829
	.loc 1 1553 5 view .LVU830
	.loc 1 1553 7 is_stmt 0 view .LVU831
	mov	r0, r4
.LVL210:
.L166:
	.loc 1 1558 1 view .LVU832
	add	sp, sp, #12
.LCFI44:
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.LVL211:
.L169:
.LCFI45:
	.loc 1 1555 7 view .LVU833
	mov	r0, #-1
.LVL212:
	.loc 1 1557 3 is_stmt 1 view .LVU834
	.loc 1 1557 10 is_stmt 0 view .LVU835
	b	.L166
.L171:
	.align	2
.L170:
	.word	_SEGGER_RTT
.LFE231:
	.size	SEGGER_RTT_SetFlagsUpBuffer, .-SEGGER_RTT_SetFlagsUpBuffer
	.section	.text.SEGGER_RTT_SetFlagsDownBuffer,"ax",%progbits
	.align	1
	.global	SEGGER_RTT_SetFlagsDownBuffer
	.syntax unified
	.thumb
	.thumb_func
	.type	SEGGER_RTT_SetFlagsDownBuffer, %function
SEGGER_RTT_SetFlagsDownBuffer:
.LVL213:
.LFB232:
	.loc 1 1576 73 is_stmt 1 view -0
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	.loc 1 1577 3 view .LVU837
	.loc 1 1579 3 view .LVU838
	.loc 1 1579 3 view .LVU839
	.loc 1 1576 73 is_stmt 0 view .LVU840
	push	{r0, r1, r2, r4, r5, r6, r7, lr}
.LCFI46:
	.loc 1 1579 3 view .LVU841
	ldr	r5, .L176
	ldrb	r3, [r5]	@ zero_extendqisi2
	.loc 1 1576 73 view .LVU842
	mov	r6, r0
	mov	r7, r1
	.loc 1 1579 3 view .LVU843
	cbnz	r3, .L173
	.loc 1 1579 3 is_stmt 1 discriminator 1 view .LVU844
	bl	_DoInit
.LVL214:
.L173:
	.loc 1 1579 3 discriminator 3 view .LVU845
	.loc 1 1580 3 discriminator 3 view .LVU846
	.loc 1 1580 6 is_stmt 0 discriminator 3 view .LVU847
	ldr	r3, [r5, #20]
	cmp	r3, r6
	bls	.L175
.LBB17:
	.loc 1 1581 5 is_stmt 1 view .LVU848
	movs	r4, #0
	add	r0, sp, #7
	strb	r4, [sp, #7]
	.loc 1 1581 5 view .LVU849
	bl	app_util_critical_region_enter
.LVL215:
	.loc 1 1581 22 view .LVU850
	.loc 1 1582 5 view .LVU851
	.loc 1 1582 42 is_stmt 0 view .LVU852
	movs	r3, #24
	mla	r5, r3, r6, r5
	.loc 1 1583 5 view .LVU853
	ldrb	r0, [sp, #7]	@ zero_extendqisi2
	.loc 1 1582 42 view .LVU854
	str	r7, [r5, #92]
	.loc 1 1583 5 is_stmt 1 view .LVU855
	bl	app_util_critical_region_exit
.LVL216:
.LBE17:
	.loc 1 1583 24 view .LVU856
	.loc 1 1584 5 view .LVU857
	.loc 1 1584 7 is_stmt 0 view .LVU858
	mov	r0, r4
.LVL217:
.L172:
	.loc 1 1589 1 view .LVU859
	add	sp, sp, #12
.LCFI47:
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.LVL218:
.L175:
.LCFI48:
	.loc 1 1586 7 view .LVU860
	mov	r0, #-1
.LVL219:
	.loc 1 1588 3 is_stmt 1 view .LVU861
	.loc 1 1588 10 is_stmt 0 view .LVU862
	b	.L172
.L177:
	.align	2
.L176:
	.word	_SEGGER_RTT
.LFE232:
	.size	SEGGER_RTT_SetFlagsDownBuffer, .-SEGGER_RTT_SetFlagsDownBuffer
	.section	.text.SEGGER_RTT_Init,"ax",%progbits
	.align	1
	.global	SEGGER_RTT_Init
	.syntax unified
	.thumb
	.thumb_func
	.type	SEGGER_RTT_Init, %function
SEGGER_RTT_Init:
.LFB233:
	.loc 1 1600 29 is_stmt 1 view -0
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	.loc 1 1601 3 view .LVU864
	b	_DoInit
.LVL220:
.LFE233:
	.size	SEGGER_RTT_Init, .-SEGGER_RTT_Init
	.section	.text.SEGGER_RTT_SetTerminal,"ax",%progbits
	.align	1
	.global	SEGGER_RTT_SetTerminal
	.syntax unified
	.thumb
	.thumb_func
	.type	SEGGER_RTT_SetTerminal, %function
SEGGER_RTT_SetTerminal:
.LVL221:
.LFB234:
	.loc 1 1618 46 view -0
	@ args = 0, pretend = 0, frame = 8
	@ f