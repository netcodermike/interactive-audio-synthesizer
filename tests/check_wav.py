import sys,wave,struct
with wave.open(sys.argv[1],'rb') as w:
    assert w.getnchannels()==1 and w.getsampwidth()==2 and w.getframerate()==44100
    assert w.getnframes()==132300
    samples=struct.unpack('<'+'h'*w.getnframes(),w.readframes(w.getnframes()))
    assert max(samples)>1000 and min(samples)<-1000
    assert max(abs(x) for x in samples)<=32767
print('WAV format, duration, and audio signal checks passed')
