.global tiles_mcz
.type tiles_mcz STT_COMMON

.section .rodata
.balign 4

    tiles_mcz: .incbin "tiles.mcz",12
    annie_mcz: .incbin "annie.mcz",12
    animz_mcz: .incbin "animz.mcz",12
    bitfont_bin: .incbin "bitfont.bin"
