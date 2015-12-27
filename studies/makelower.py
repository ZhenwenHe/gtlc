import os 

strp = 'h:/git/gtl/gdb/kernel/core/src'
fa = os.listdir(strp)
for s in fa:
	if(s.islower()):
		print(s)
	else: 		
		s1= s.lower()		
		s=strp + '/' + s;
		s1=strp + '/' + s1;
		os.rename(s,s1) 
		