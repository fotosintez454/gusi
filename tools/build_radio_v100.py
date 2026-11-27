"""Original instrumental loops; no samples, vocals, melodies or artist recordings copied."""
from pathlib import Path
import numpy as np
import wave
R=Path(__file__).resolve().parents[1]
SR=22050
rng=np.random.default_rng(100)
def hz(m):return 440*2**((m-69)/12)
for name,bpm,roots in [('02_Pustoy_dvor_original',104,[40,36,43,38]),('03_Posledniy_avtobus_original',118,[45,41,48,43])]:
 beat=60/bpm;duration=beat*4*32;n=int(SR*duration);out=np.zeros(n,dtype=np.float64)
 def add(start,sound,gain):
  i=int(start*SR);end=min(n,i+len(sound))
  if i<n:out[i:end]+=sound[:end-i]*gain
 for bar in range(32):
  root=roots[(bar//2)%4];base=bar*4*beat
  for k in range(8):
   t=np.arange(int(SR*beat*.45))/SR;freq=hz(root+(12 if k in (3,7) else 0));env=np.minimum(1,t/.008)*np.exp(-t*5)
   bass=np.tanh(1.8*(np.sin(2*np.pi*freq*t)+.22*np.sin(4*np.pi*freq*t)))*env
   add(base+k*beat/2,bass,.17)
   t=np.arange(int(SR*.09))/SR;noise=rng.normal(size=len(t));high=np.r_[0,np.diff(noise)]*np.exp(-t*50);add(base+k*beat/2,high,.025 if k%2 else .04)
  for k in (0,2):
   t=np.arange(int(SR*.27))/SR;phase=2*np.pi*(46*t+65*.025*(1-np.exp(-t/.025)));add(base+k*beat,np.sin(phase)*np.exp(-t*17),.36)
  for k in (1,3):
   t=np.arange(int(SR*.19))/SR;noise=rng.normal(size=len(t));snare=(noise*.6+np.sin(2*np.pi*175*t)*.4)*np.exp(-t*22);add(base+k*beat,snare,.18)
  # Sparse picked minor voicings with a short tape-like echo.
  for k,interval in enumerate([12,19,15,22,19,15,24,19]):
   t=np.arange(int(SR*beat*1.7))/SR;f=hz(root+interval);guitar=sum(np.sin(2*np.pi*f*j*t)*np.exp(-t*(2+j*.9))/j for j in range(1,6));guitar*=np.minimum(1,t/.006)
   start=base+k*beat/2;add(start,guitar,.052);add(start+beat*.75,guitar,.017);add(start+beat*1.5,guitar,.007)
  if bar>=8:
   t=np.arange(int(SR*beat*4))/SR
   pad=sum(np.sin(2*np.pi*hz(root+v)*t+.006*np.sin(t*3.1)) for v in (12,15,19))/3
   pad*=np.minimum(1,t/.3)*np.minimum(1,(len(t)/SR-t)/.4);add(base,pad,.045)
 fade=np.minimum(1,np.arange(n)/SR/1.5)*np.minimum(1,(n-np.arange(n))/SR/3);out=np.tanh(out*1.2)*fade
 pcm=(out/ max(1,np.max(np.abs(out))/.86)*32767).astype('<i2')
 path=R/'radio'/f'{name}.wav'
 with wave.open(str(path),'wb') as w:w.setparams((1,2,SR,0,'NONE','not compressed'));w.writeframes(pcm.tobytes())
 print(path.name,round(duration,2),'seconds')
(R/'radio'/'README_RU.txt').write_text('РАДИО ОКНО 0.10.0\n\n02 Пустой двор и 03 Последний автобус — оригинальные синтезированные инструменталы: бас, минорная гитара, сухие барабаны. Не записи и не каверы Буерака или Кино. Созданы без чужих аудиосэмплов.\n\nДля своих треков: положите законно полученные MP3 или PCM WAV в эту папку рядом с EXE. Перезапустите игру для обновления списка. Кириллица поддерживается. Переключение — ручками домашнего радиоприёмника (ЛКМ). Файлы Буерака/Кино не включены.\n',encoding='utf-8')
