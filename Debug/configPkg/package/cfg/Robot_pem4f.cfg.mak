# invoke SourceDir generated makefile for Robot.pem4f
Robot.pem4f: .libraries,Robot.pem4f
.libraries,Robot.pem4f: package/cfg/Robot_pem4f.xdl
	$(MAKE) -f C:\Users\jingwen\Desktop\MRESRO~1\GX02-Control\CSRobot_01_2017/src/makefile.libs

clean::
	$(MAKE) -f C:\Users\jingwen\Desktop\MRESRO~1\GX02-Control\CSRobot_01_2017/src/makefile.libs clean

