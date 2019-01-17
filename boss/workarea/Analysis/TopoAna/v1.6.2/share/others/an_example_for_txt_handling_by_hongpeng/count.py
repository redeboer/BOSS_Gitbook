f1 =open("List1")
INC =open("INC.txt")
ob= open("List2","w")
INCall=INC.read()
inc=INCall.split("nEvts:")
f1lines=f1.readlines()
for ist in f1lines:

	tot=0
	for ist1 in inc:
		if ist in ist1:
			a=ist1[2:4]
			tot+=long(a)
	a=str(tot)
	ob.write(a+"  ")
	ob.write(ist)
	ob.write("\n")

f1.close()
