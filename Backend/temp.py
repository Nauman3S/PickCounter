a=open('client_secret.json','r')
g=a.read()
a.close()

kk=open('cs.bin','ab')
kk.write(g.encode())
kk.close()

mm=open('cs.bin','rb')
ggg=mm.read()
mm.close()

print(ggg)