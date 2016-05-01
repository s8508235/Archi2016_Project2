import os
import subprocess
import sys
id = raw_input("Please enter your student id:")
hidden_list=[]
student_list=[]
htpath = "/home/archi/s"+str(id) + "/Archi2016_Project1/" + "pipeline/test/hidden_testcase"
stpath = "/home/archi/s"+str(id) + "/Archi2016_Project1/" + "pipeline/test/Student_valid_testcase"
gpath =  "/home/archi/s"+str(id) + "/Archi2016_Project2/" + "archiTA/simulator"
def build_list(inputpath):
	out_list=[]
	for dirP,dirN,FN is os.walk(inputpath):
		out_list.append(dirP)
	return out_list
def build_test(file_list):
	for filepath in file_list:
		if(filepath !=file_list[0]):
			gipath = filepath +'/iimage.bin'
			gdpath = filepath +'/dimage.bin'
			gicom = 'cp ' + gipath+' '+gpath
			gdcom = 'cp ' + gdpath+' '+gpath
			gicp = os.system(gicom)
			gdcp = os.system(gdcom)
			if(gicp ==0 and gdcp == 0):
				print("copy files to golden correct!")
			gfilename = filepath.split('/',7)[7]
			subprocess.call(['make','clean'],shell=False,stdout=subprocess.PIPE)
			subprocess.call('make',shell=False,stdout=subprocess.PIPE)
			os.system(gpath+'/pipeline')
			gcomi ='cp '+gpath+'/snapshot.rpt'+' '+filepath
			gcomd ='cp '+path+'/error_dump.rpt'+' '+filepath
			os.system(gcomi)
			os.system(gcomd)	 
def file_test(file_list):
	for filepath in file_list:
		if(filepath != file_list[0]):
			ipath = filepath+'/'+'iimage.bin'
			dpath = filepath+'/'+'dimage.bin'
			if( os.path.isfile and  os.path.isfile(dpath)):
				print ("Files exist") 
			icom = 'cp ' + ipath+' .'
			dcom = 'cp ' + dpath+' .'
			icp = os.system(icom)
			dcp = os.system(dcom)
			if(icp ==0 and  dcp ==0):
				print ("copy correct")
			ifilename = filepath.split('/',8)[8]
			subprocess.call(['make','clean'],shell=False,stdout = subprocess.PIPE)
			subprocess.call('make',shell=False,stdout = subprocess.PIPE)
			os.system('./single_cycle')
			gipath = 'cp '+ipath+' '+gpath
			gdpath = 'cp '+dpath+' '+gpath
			os.system(gipath)
			os.system(gdpath)
			os.system(gpath+' ./pipeline')
			idiffcom = 'diff '+gpath+'/snapshot.rpt '+filepath+'/snapshot.rpt' 
			idiff = os.system(idiffcom)
			if(idiff ==0):
				print ("Snapshot Correct!")
			else:
				print ("Error or Wrong")
				print 'testing :' +ifilename
				return ;
			ddiffcom = 'diff '+gpath+'/error_dump.rpt '+filepath+'/error_dump.rpt'
			ddiff = os.system(ddiffcom)
			if(ddiff==0):
				print ("Error_dump Correct!")
			else:
				print ("Error or Wrong")	
				print 'testing :' +ifilename
				return ;
			print 'testing :' +ifilename
hidden_list=build_list(htpath)
print "start:" +hidden_list[0]
build_test(hidden_list)
file_test(hidden_list)
student_list = build_list(stpath)
print "start:" +student_list[0]
build_test(student_list)
file_test(student_list)
