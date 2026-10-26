"""Extract original Palm PRC resources. No proprietary Palm ROM required."""
import struct,json,zlib,binascii
from pathlib import Path
root=Path(__file__).resolve().parent
b=(root.parent/'SpaceTrader_color.prc').read_bytes()
u16=lambda b,o:struct.unpack_from('>H',b,o)[0]
u32=lambda b,o:struct.unpack_from('>I',b,o)[0]
entries=[(b[78+i*10:82+i*10].decode('latin1'),u16(b,82+i*10),u32(b,84+i*10)) for i in range(u16(b,76))]
r={(t,n):b[o:entries[i+1][2] if i+1<len(entries) else len(b)] for i,(t,n,o) in enumerate(entries)}
def st(b,o):return b[o:].split(b'\0')[0].decode('mac_roman')
forms={}; alerts={};strings={};lists={}
for (t,n),d in r.items():
 if t=='tFRM':
  f=dict(id=n,x=u16(d,10),y=u16(d,12),w=u16(d,14),h=u16(d,16),default=u16(d,56),menu=u16(d,60),title='',objects=[])
  for j in range(u16(d,62)):
   typ=d[68+j*6];o=u32(d,70+j*6);a=d[o:]
   q=dict(type=typ,id=0,x=0,y=0,w=0,h=0,font=0,style=0,text='',hidden=False)
   if typ in [0,1,2]:
    q.update(zip(['id','x','y','w','h'],struct.unpack_from('>5H',a)))
    if typ==1:q.update(font=a[17],style=a[16],text=st(a,20),hidden=not(u16(a,14)&0x8000))
    if typ==0:q.update(font=a[38],hidden=not(u16(a,10)&0x8000))
    if typ==2:
     count=u16(a,16);q['items']=a[32+count*4:].split(b'\0')[:count];q['items']=[s.decode('mac_roman') for s in q['items']];q['hidden']=True
   elif typ==8:q.update(id=u16(a,0),x=u16(a,2),y=u16(a,4),font=a[8],text=st(a,14),hidden=not(u16(a,6)&0x8000))
   elif typ==9:f['title']=st(a,12)
   elif typ==4:q.update(id=u16(a,6),x=u16(a,2),y=u16(a,4),hidden=not(u16(a,0)&0x8000))
   elif typ==10:q.update(id=u16(a,0),list=u16(a,2))
   f['objects'].append(q)
  forms[str(n)]=f
 elif t=='Talt':
  ss=d[8:].split(b'\0'); alerts[str(n)]={'title':ss[0].decode('mac_roman'),'message':ss[1].decode('mac_roman'),'buttons':[x.decode('mac_roman') for x in ss[2:2+u16(d,4)]]}
 elif t=='tSTR':strings[str(n)]=st(d,0)
 elif t=='tSTL':
  off=d.index(0)+1;prefix=st(d,0);count=u16(d,off);lists[str(n)]=[prefix+x.decode('mac_roman') for x in d[off+2:].split(b'\0')[:count]]
(root/'Resources'/'resources.json').write_text(json.dumps(dict(forms=forms,alerts=alerts,strings=strings,lists=lists)))
# Decode the highest-depth member of each Palm bitmap family.
def png(path,w,h,pixels):
 def chunk(t,d):return struct.pack('>I',len(d))+t+d+struct.pack('>I',binascii.crc32(t+d)&0xffffffff)
 path.write_bytes(b'\x89PNG\r\n\x1a\n'+chunk(b'IHDR',struct.pack('>IIBBBBB',w,h,8,2,0,0,0))+chunk(b'IDAT',zlib.compress(b''.join(b'\0'+pixels[y*w*3:(y+1)*w*3] for y in range(h))))+chunk(b'IEND',b''))
# Palm default 8-bit palette: six-level cube with the platform grayscale tail.
pal=json.loads((root/'palm-palette.json').read_text())
for (t,n),d in r.items():
 if t not in ('Tbmp','tAIB'):continue
 off=0;members=[]
 while off+16<=len(d):
  a=d[off:];w,h,row,flags=struct.unpack_from('>4H',a);depth=a[8] or 1;ver=a[9];nxt=u16(a,10)
  members.append((depth,a));
  if not nxt:break
  off+=nxt*4
 depth,a=max(members,key=lambda v:v[0]);w,h,row,flags=struct.unpack_from('>4H',a);pos=16;palette=pal.copy()
 if flags&0x4000:
  count=u16(a,pos);pos+=2
  for i in range(count):idx,rr,gg,bb=a[pos:pos+4];palette[idx]=(rr,gg,bb);pos+=4
 raw=a[pos:];out=bytearray(row*h)
 if flags&0x8000:
  k=2;comp=a[13] if a[9]>=2 else 0
  if comp==0:
   for y in range(h):
    for x in range(0,row,8):
     mask=raw[k];k+=1
     for bit in range(min(8,row-x)):
      idx=y*row+x+bit
      if mask&(128>>bit):out[idx]=raw[k];k+=1
      elif y:out[idx]=out[idx-row]
  elif comp==1:
   j=0
   while j<len(out):count,val=raw[k:k+2];k+=2;out[j:j+count]=bytes([val])*count;j+=count
  else:raise ValueError(('compression',n,comp))
 else:out[:]=raw[:row*h]
 pixels=bytearray()
 for y in range(h):
  for x in range(w):
   val=(out[y*row+x*depth//8]>>(8-depth-(x*depth%8)))&((1<<depth)-1)
   rgb=palette[val] if depth==8 else (255-val*255//((1<<depth)-1),)*3
   pixels.extend(rgb)
 png(root/'Resources'/f'{n}.png',w,h,pixels)
print('Extracted',len(forms),'forms,',len(alerts),'alerts,',len(strings),'strings;',len(list((root/'Resources').glob('*.png'))),'bitmaps')
