.global tiles_mcz
.global annie_mcz
.global animz_mcz
.global bitfont_bin
.global levels_mapz
.type tiles_mcz STT_COMMON
.type annie_mcz STT_COMMON
.type bitfont_bin STT_COMMON

.section .rodata
.balign 4

    tiles_mcz: .incbin "tiles.mcz",12
    annie_mcz: .incbin "annie.mcz",12
    animz_mcz: .incbin "animz.mcz",12
    bitfont_bin: .incbin "bitfont.bin"
    levels_mapz: .incbin "levels.mapz"
