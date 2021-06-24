
## MIDI Spec

### Note ON
- channel, note# は無視
- 光りの演出を開始する(10steps * duration)
- velocityにより輝度(HSVのV)を変更: 0-127 を 0-255 にマップする

### Control Change
- channel は無視
- CC2 = duration変更: 0-127 を 10-1000 にマップする
- CC3 = hue変更(HSVのH): 0-127 を 0-360 にマップする