from pathlib import Path
from PIL import Image, ImageEnhance, ImageFilter, ImageDraw
import random, math, re
R=Path(__file__).resolve().parents[1]; A=R/'art/postpunk170';A.mkdir(exist_ok=True)
rng=random.Random(170)
def mip(im):
 a=[]
 while True:
  a.extend((r<<16)|(g<<8)|b for r,g,b in im.getdata())
  if im.width==1: break
  im=im.resize((im.width//2,im.height//2),Image.Resampling.BOX)
 return a
def header(name,im,repeat=1):
 return 'static const U32 '+name+'_pixels[87381]={'+','.join(hex(v) for v in mip(im))+'};\nstatic const Texture '+name+'={0,256,256,'+str(repeat)+','+name+'_pixels};\n'
# New restrained grades preserve the original supplied material detail rather than synthetic placeholder art.
h=['/* v170: regraded source materials; full CPU/OpenGL mip chains. */\n']
for name,tint,sat,brightness in [('wallpaper',(154,161,151),.22,1.30),('parquet',(137,123,103),.40,1.18),('wood',(128,113,91),.38,1.30),('tiles',(170,179,168),.12,1.18)]:
 im=Image.open(R/'art/reference_pass'/f'{name}.png').convert('RGB').resize((256,256))
 im=ImageEnhance.Color(im).enhance(sat); im=ImageEnhance.Brightness(im).enhance(brightness)
 im=Image.blend(im,Image.new('RGB',im.size,tint),.22)
 if name=='wallpaper':
  # Small-scale faded woven stripe, no enormous repeating floral blobs.
  im=im.filter(ImageFilter.GaussianBlur(.35)); d=ImageDraw.Draw(im,'RGB')
  for x in range(0,256,32): d.line((x,0,x,255),fill=(142,150,140),width=1)
 im.save(A/f'{name}.png');h.append(header('art_'+name,im,2 if name=='wallpaper' else 1))
(R/'src/room_art.h').write_text(''.join(h))
# Concrete source comes from supplied material; regenerate with controlled damp streaks and aggregate.
s=(R/'materials.h').read_text(); part=s.split('px_concrete[]={',1)[1].split('};',1)[0]; vals=[int(x,16) for x in re.findall(r'0x[0-9a-fA-F]+',part)[:65536]]
im=Image.new('RGB',(256,256)); im.putdata([((x>>16)&255,(x>>8)&255,x&255) for x in vals]); im=ImageEnhance.Color(im).enhance(.08); im=ImageEnhance.Contrast(im).enhance(.42)
im=Image.blend(im,Image.new('RGB',im.size,(158,163,159)),.4)
pix=im.load()
for y in range(256):
 for x in range(256):
  r,g,b=pix[x,y]; grain=rng.randrange(-3,4); stain=-int(5*(.5+.5*math.sin(x*.11+math.sin(y*.021))))
  pix[x,y]=tuple(max(0,min(255,c+grain+stain)) for c in (r,g,b))
im.save(A/'concrete.png');(R/'src/postpunk_materials.h').write_text(header('art_concrete170',im))
(A/'PROVENANCE_RU.txt').write_text('Материалы v170: новая обработка и производные от текстур исходного проекта. Бетон: исходная px_concrete, приглушение цвета, мелкое зерно и следы влаги. Обои, паркет, дерево, кафель: исходные art/reference_pass с новой цветокоррекцией. Внешние ассеты и музыка не добавлялись. Процедурные архитектурные модели созданы для этого обновления.\n')
# Export editable companion meshes for new runtime modules (metres, Y up).
class Obj:
 def __init__(self): self.lines=['# OKNO v170 original modular architecture; metres, Y up'];self.n=0
 def box(self,name,x,y,z,w,h,d):
  self.lines+=['o '+name]
  for a,b,c in [(-1,-1,-1),(1,-1,-1),(1,1,-1),(-1,1,-1),(-1,-1,1),(1,-1,1),(1,1,1),(-1,1,1)]: self.lines.append(f'v {x+a*w/2:.5f} {y+b*h/2:.5f} {z+c*d/2:.5f}')
  for f in [(1,4,3,2),(5,6,7,8),(1,5,8,4),(2,3,7,6),(4,8,7,3),(1,2,6,5)]: self.lines.append('f '+' '.join(str(self.n+i) for i in f))
  self.n+=8
 def save(self,name): (A/name).write_text('\n'.join(self.lines)+'\n')
o=Obj()
for i in range(7):o.box('cast_iron_section_'+str(i),(i-3)*.115,.51,0,.092,.65,.18)
o.box('feed_pipe',0,.80,-.08,1.0,.045,.045);o.box('return_pipe',0,.22,-.08,1,.045,.045);o.save('radiator_7_sections.obj')
o=Obj()
for e in [-1,1]:o.box('jamb',e*.78,1.225,0,.085,2.45,.24)
o.box('lintel',0,2.48,0,1.65,.08,.24);o.box('leaf',0,1.215,0,1.48,2.40,.05);o.save('door_assembly.obj')
o=Obj();o.box('housing',0,0,0,.40,.24,.09);o.box('diffuser',0,0,-.065,.31,.16,.045)
for i in [-1,0,1]:o.box('guard',i*.105,0,-.10,.018,.22,.018)
o.save('stairwell_lamp.obj')
print('Created materials and OBJ modules',A)
