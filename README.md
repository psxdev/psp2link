PSP2LINK FOR VITA
=================
 
===================
 What does this do?
===================
 
  psp2link is a library for vita to communicate and use host file system with psp2client host tool. It is the same method that we used in ps2dev days, so basically it is the same protocol than ps2link and ps2client have been using since 2003.
  
  Functions availables are defined like native sceIoxx functions so it is easy for homebrew developer to use these new functions:
  ```
  int psp2LinkIoOpen(const char *file, int flags, SceMode mode);
  int psp2LinkIoClose(SceUID fd);
  int psp2LinkIoRead(SceUID fd, void *data, SceSize size);
  int psp2LinkIoWrite(SceUID fd, const void *data, SceSize size);
  int psp2LinkIoLseek(SceUID fd, int offset, int whence);
  int psp2LinkIoRemove(const char *file);
  int psp2LinkIoMkdir(const char *dirname, SceMode mode);
  int psp2LinkIoRmdir(const char *dirname);
  int psp2LinkIoDopen(const char *dirname);
  int psp2LinkIoDread(SceUID fd, SceIoDirent *dir);
  int psp2LinkIoDclose(SceUID fd);
  ```
  
  
==================
  How do I use it?
==================

 1) Compile and install library and include file

  You need the last vita toolchain installed in your environment ,libdebugnet and libvita2d installed and VITASDK must be defined.

  
  ```
  cd libpsp2link
  make
  make install
  ```
  
 2) Compile sample
  
  ```
  cd sample
  edit main.c and change your mac/linux server ip and your base directory for example host0:/usr/local
  make
  ```

 3) Run sample with rejuvenete on vita and when you see psp2link splash screen you are ready to run psp2client in your mac/linux
   
  ```
  psp2client -h ipofyourvita listen 
  ```
  
  Command protocol is implemented and you can extend it with your custom code. Examples can be run with:
  
  ```
  psp2client -h ipofyourvita -t 5 execelf host0:filename 
  ```
  
  ```
  psp2client -h ipofyourvita -t 5 execsprx host0:filename 
  ```
  
  ```
  psp2client -h ipofyourvita -t 5 exit
  ```
 
 You will see logs in terminal window output from your PlayStation Vita using libdebugnet udp log feauture and you can browse over folder in your hard disk.
 
 Sample is showing only browser example you have full io r/w access to your hard disk and you can test how commands can be implemented
 
 ```
 ./psp2client -h yourvitaip listen
 [INFO]: Client connected from xxx.xxx.xxx.xxx port: 30915
  [INFO]: sock psp2link_fileio set 50 connected 1
 [INFO]: Waiting for connection
 [INFO]: psp2link connected  1
 [DEBUG]: dir open req (host0:/usr/local)
 [DEBUG]: dir open reply received (ret 0)
 [DEBUG]: dir read req (0)
 [DEBUG]: dir read reply received (ret 1)
 [DEBUG]: dir read req (0)
 [DEBUG]: dir read reply received (ret 1)
 [DEBUG]: dir read req (0)
 [DEBUG]: dir read reply received (ret 1)
 [DEBUG]: dir read req (0)
 [DEBUG]: dir read reply received (ret 1)
 [DEBUG]: dir read req (0)
 [DEBUG]: dir read reply received (ret 1)
 [DEBUG]: dir read req (0)
 [DEBUG]: dir read reply received (ret 1)
 [DEBUG]: dir read req (0)
 [DEBUG]: dir read reply received (ret 1)
 [DEBUG]: dir read req (0)
 [DEBUG]: dir read reply received (ret 1)
 [DEBUG]: dir read req (0)
 [DEBUG]: dir read reply received (ret 1)
 [DEBUG]: dir read req (0)
 [DEBUG]: dir read reply received (ret 1)
 [DEBUG]: dir read req (0)
 [DEBUG]: dir read reply received (ret 1)
 [DEBUG]: dir read req (0)
 [DEBUG]: dir read reply received (ret 1)
 [DEBUG]: dir read req (0)
 [DEBUG]: dir read reply received (ret 1)
 [DEBUG]: dir read req (0)
 [DEBUG]: dir read reply received (ret 1)
 [DEBUG]: dir read req (0)
 [DEBUG]: dir read reply received (ret 1)
 [DEBUG]: dir read req (0)
 [DEBUG]: dir read reply received (ret 1)
 [DEBUG]: dir read req (0)
 [DEBUG]: dir read reply received (ret 1)
 [DEBUG]: dir read req (0)
 [DEBUG]: dir read reply received (ret 1)
 [DEBUG]: dir read req (0)
 [DEBUG]: dir read reply received (ret 1)
 [DEBUG]: dir read req (0)
 [DEBUG]: dir read reply received (ret 0)
 [DEBUG]: psp2link_file: dir close req (fd: 0)
 [DEBUG]: dir close reply received (ret 0)
 ```
 
 ```
 ./psp2client -h yourvitaip -t 10 execelf host0:README.md
 [ERROR]: [PSP2LINK] Client reconnected
 [INFO]: [PSP2LINK] sock psp2link_fileio set 124 connected 1
 [INFO]: [PSP2LINK] Waiting for connection
 [DEBUG]: [PSP2LINK] commands listener received packet size (266)
 [DEBUG]: [PSP2LINK] Received command execelf argc=1 argv=host0:README.md
 [DEBUG]: [PSP2LINK] file open req (host0:README.md, 1 0)
 Opening README.md flags 0
 Open return 5
 [DEBUG]: [PSP2LINK] file open reply received (ret 5)
 [DEBUG]: [PSP2LINK] file lseek req (fd: 5)
 3382 result of lseek 0 offset 2 whence
 [DEBUG]: [PSP2LINK] psp2link_lseek_file: lseek reply received (ret 3382)
 [DEBUG]: [PSP2LINK] file lseek req (fd: 5)
 0 result of lseek 0 offset 0 whence
 [DEBUG]: [PSP2LINK] psp2link_lseek_file: lseek reply received (ret 0)
 [DEBUG]: [PSP2LINK] psp2link_read_file: Reply said there's 3382 bytes to read (wanted 3382)
 [DEBUG]: [PSP2LINK] psp2link_file: file close req (fd: 5)
 [DEBUG]: [PSP2LINK] psp2link_close_file: close reply received (ret 0)
 [DEBUG]: [PSP2LINK] commands listener waiting for next command
 ```
 
 
 5) ready to have a lot of fun :P
 
===================
 What next?
===================
  
  Well, this library can be extended adding differents commands like ps2link was defined. It could be incorporated inside uvloader code and it would let load elf from hosts and provide host io fuctionality to homebrew apps. I am not going to do a loader when we have already uvloader backend. If it is not incorporated inside uvloader you can use like library showed in sample.
  
  About logging i like more udp log system like libdebugnet instead logging system incorporated in uvloader, but it is only my preference based only in my own experience since ps2dev days.
  
  If you don't want to use logging, initialize psp2link with right loglevel check libdebugnet sources to see differents log levels availables.
  
===========================
  Credits
===========================
  
  Special thanks goes to:
  
  - ps2dev old comrades. 
  - All people who collaborated in PSP2SDK: @173210, @xerpi, @frangar, @frtomtomdu80, @hykemthedemon , @SMOKE587, @Josh_Axey ... 
  - font in logo from http://www.fontspace.com/nal/the-rave-is-in-your-pants
  - xyzz for help with new toolchain
  
