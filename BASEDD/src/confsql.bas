/*X  Fichier : $Id: confsql.bas,v 1.8 1996/08/21 18:31:13 vathelot Exp $      Release : $Revision: 1.8 $        Date : $Date: 1996/08/21 18:31:13 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER confsql.bas
------------------------------------------------------
* DESCRIPTION :
* cree les devices
* cree le user applicatif MIGRAZUR
* cree les bases
* 
* 
*
------------------------------------------------------
* HISTORIQUE :
*
* B.GABORIT	09/08/94	: Creation
* B.GABORIT	11/08/94	: Ajout de la base RSSD
* erreur de manip sccs pour 1.4
* B.GABORIT	14/09/94	: Ajout de la base CFT
* B.GABORIT	02/11/94	: Ajout de la base SVG
* C.TORREGROSSA	21/03/95	: Modif noms des volumes et
*                                 tailles des bases (V 1.7)
* L.VATHELOT	21/08/95	: Ajout de la base VUE
* ce script n'est plus utilise : il sert de recapitulatif
----------------------------------------------------*/

use master
go

-----------------------------------------------------
-- ajout du login MIGRAZUR
-----------------------------------------------------
exec sp_addlogin MIGRAZUR, MIGRAZUR, null, null, "login applicatif Migrazur"
print "ajout du login MIGRAZUR : OK"
go
 
-----------------------------------------------------
-- variables pour diskinit
-----------------------------------------------------
declare @dernier_vdevno int
declare @taille int

-----------------------------------------------------
-- serveur SQL_CI
-----------------------------------------------------
if @@servername = 'SQL_CI'
   begin
   print "configuration des devices du CI"
   
   -- ajout du device EXP_dev
   if not exists (select name from sysdevices where name="EXP_dev")
      begin
      print "ajout du device EXP_dev"
      select @dernier_vdevno=max(low/16777216)+1 from sysdevices
      select @taille=512*512 --512 meg
      disk init
         name="EXP_dev",
         physname="/dev/vg00/lvol8",
         vdevno=@dernier_vdevno,
         size=@taille
      end
   print "device EXP_dev : OK"
   

   -- ajout du device HIS_dev
   if not exists (select name from sysdevices where name="HIS_dev")
      begin
      print "ajout du device HIS_dev"
      select @dernier_vdevno=max(low/16777216)+1 from sysdevices
      select @taille=1500*512 --1500 meg
      disk init
         name="HIS_dev",
         physname="/dev/vg01/lvol1",
         vdevno=@dernier_vdevno,
         size=@taille
      end
   print "device HIS_dev : OK"

   -- ajout de la base EXP
   print "ajout de la base EXP"
   create database EXP
      on EXP_dev=50 -- 50 meg
      log on HIS_dev=100 --100 meg sur autre device
   use EXP
   exec sp_changedbowner MIGRAZUR
   use master
   print "base EXP : OK"
   
   -- ajout de la base CFG
   print "ajout de la base CFG"
   create database CFG
      on HIS_dev=50 -- 50 meg
      log on EXP_dev=50 --50 meg sur autre device
   use CFG
   exec sp_changedbowner MIGRAZUR
   use master
   print "base CFG : OK"
   
   -- ajout de la base CFT
   print "ajout de la base CFT"
   create database CFT
      on EXP_dev=50 -- 50 meg
      log on HIS_dev=50 --50 meg sur autre device
   use CFT
   exec sp_changedbowner MIGRAZUR
   use master
   print "base CFT : OK"
   
   -- ajout de la base HIS
   print "ajout de la base HIS"
   create database HIS
      on HIS_dev=1000 -- 1000 meg
      log on EXP_dev=100 --100 meg sur autre device
   use HIS
   exec sp_changedbowner MIGRAZUR
   use master
   print "base HIS : OK"
   
   -- ajout de la base CI_RSSD
   print "ajout de la base CI_RSSD"
   create database CI_RSSD
      on EXP_dev=10 -- 10 meg
      log on HIS_dev=10 --10 meg sur autre device
   print "base CI_RSSD : OK"
   
   -- ajout de la base PRC
   print "ajout de la base PRC"
   create database PRC
      on EXP_dev=50 -- 50 meg
   use PRC
   exec sp_changedbowner MIGRAZUR
   use master
   print "base PRC : OK"

   -- ajout de la base SVG
   print "ajout de la base SVG"
   create database SVG
      on EXP_dev=50 -- 50 meg
      log on HIS_dev=50 --50 meg sur autre device
   use SVG
   exec sp_changedbowner MIGRAZUR
   use master
   print "base SVG : OK"
  
  -- ajout de la base VUE
  print "ajout de la base VUE"
  create database VUE
  	on EXP_dev=5 -- 5 meg
  use VUE
  exec sp_changedbowner MIGRAZUR
  use master
  print "base VUE : OK"

   end

-----------------------------------------------------
-- serveur SQL_DN
-----------------------------------------------------
else if @@servername = 'SQL_DN'
   begin
   print "configuration des devices du DN"
   
   -- ajout du device EXP_dev
   if not exists (select name from sysdevices where name="EXP_dev")
      begin
      print "ajout du device EXP_dev"
      select @dernier_vdevno=max(low/16777216)+1 from sysdevices
      select @taille=320*512 --320 meg
      disk init
         name="EXP_dev",
         physname="/dev/vg00/lvol7",
         vdevno=@dernier_vdevno,
         size=@taille
      end
   print "device EXP_dev : OK"
   
   -- ajout du device CFG_dev
   if not exists (select name from sysdevices where name="CFG_dev")
      begin
      print "ajout du device CFG_dev"
      select @dernier_vdevno=max(low/16777216)+1 from sysdevices
      select @taille=320*512 --320 meg
      disk init
         name="CFG_dev",
         physname="/dev/vg00/lvol8",
         vdevno=@dernier_vdevno,
         size=@taille
      end
   print "device CFG_dev : OK"

   -- ajout de la base EXP
   print "ajout de la base EXP"
   create database EXP
      on EXP_dev=50 -- 50 meg
      log on CFG_dev=100 --100 meg sur autre device
   use EXP
   exec sp_changedbowner MIGRAZUR
   use master
   print "base EXP : OK"
   
   -- ajout de la base CFG
   print "ajout de la base CFG"
   create database CFG
      on CFG_dev=50 -- 50 meg
      log on EXP_dev=50 --50 meg sur autre device
   use CFG
   exec sp_changedbowner MIGRAZUR
   use master
   print "base CFG : OK"
   
   -- ajout de la base DN_RSSD
   print "ajout de la base DN_RSSD"
   create database DN_RSSD
      on EXP_dev=10 -- 10 meg
      log on CFG_dev=10 --10 meg sur autre device
   print "base DN_RSSD : OK"
   
   -- ajout de la base PRC
   print "ajout de la base PRC"
   create database PRC
      on EXP_dev=50 -- 50 meg
   use PRC
   exec sp_changedbowner MIGRAZUR
   use master
   print "base PRC : OK"
   
   end

-----------------------------------------------------
-- serveur SQL_DM
-----------------------------------------------------
else if @@servername = 'SQL_DM'
   begin
   print "configuration des devices du DM"
   
   -- ajout du device EXP_dev
   if not exists (select name from sysdevices where name="EXP_dev")
      begin
      print "ajout du device EXP_dev"
      select @dernier_vdevno=max(low/16777216)+1 from sysdevices
      select @taille=320*512 --320 meg
      disk init
         name="EXP_dev",
         physname="/dev/vg00/lvol7",
         vdevno=@dernier_vdevno,
         size=@taille
      end
   print "device EXP_dev : OK"
   
   -- ajout du device CFG_dev
   if not exists (select name from sysdevices where name="CFG_dev")
      begin
      print "ajout du device CFG_dev"
      select @dernier_vdevno=max(low/16777216)+1 from sysdevices
      select @taille=320*512 --320 meg
      disk init
         name="CFG_dev",
         physname="/dev/vg00/lvol8",
         vdevno=@dernier_vdevno,
         size=@taille
      end
   print "device CFG_dev : OK"

   -- ajout de la base EXP
   if exists (select name from sysdatabases where name="EXP")
      drop database EXP
   print "ajout de la base EXP"
   create database EXP
      on EXP_dev=50 -- 50 meg
      log on CFG_dev=100 --100 meg sur autre device
   use EXP
   exec sp_changedbowner MIGRAZUR
   use master
   print "base EXP : OK"
   
   -- ajout de la base CFG
   print "ajout de la base CFG"
   create database CFG
      on CFG_dev=50 -- 50 meg
      log on EXP_dev=50 --50 meg sur autre device
   use CFG
   exec sp_changedbowner MIGRAZUR
   use master
   print "base CFG : OK"
   
   -- ajout de la base DM_RSSD
   print "ajout de la base DM_RSSD"
   create database DM_RSSD
      on EXP_dev=10 -- 10 meg
      log on CFG_dev=10 --10 meg sur autre device
   print "base DM_RSSD : OK"
   
   -- ajout de la base PRC
   print "ajout de la base PRC"
   create database PRC
      on EXP_dev=50 -- 50 meg
   use PRC
   exec sp_changedbowner MIGRAZUR
   use master
   print "base PRC : OK"
   
   end
go
--------------------------------------------------------------------
-- options des bases
--------------------------------------------------------------------
use master
go
exec sp_modifylogin MIGRAZUR,defdb, PRC
go
exec sp_dboption EXP, trunc, true
go
exec sp_dboption CFG,"select into",true
go
exec sp_dboption PRC, trunc, true
go
exec sp_dboption VUE, trunc, true
go
use EXP
go
checkpoint
go
use CFG
go
checkpoint
go
use PRC
go
checkpoint
go
use VUE
go
checkpoint
go
use master
go
if @@servername = 'SQL_CI' 
begin
	exec sp_configure "memory", 1500
	exec sp_configure "user connections", 60
	exec sp_dboption CI_RSSD,trunc,true
end
go
if @@servername = 'SQL_DM' 
begin
	exec sp_configure "memory", 1024
	exec sp_configure "user connections", 60
	exec sp_dboption DM_RSSD,trunc,true
end
go
if @@servername = 'SQL_DN' 
begin
	exec sp_configure "memory", 1024
	exec sp_configure "user connections", 60
	exec sp_dboption DN_RSSD,trunc,true
end
go
reconfigure
go
if @@servername = 'SQL_CI' use CI_RSSD
go
if @@servername = 'SQL_DN' use DN_RSSD
go
if @@servername = 'SQL_DM' use DM_RSSD
go
checkpoint
go
use master
go
if @@servername = 'SQL_CI' exec sp_dboption CFT,trunc,true
go
if @@servername = 'SQL_CI' exec sp_dboption SVG,trunc,true
go
if @@servername = 'SQL_CI' use CFT
go
checkpoint
go
if @@servername = 'SQL_CI' use HIS
go
checkpoint
go
if @@servername = 'SQL_CI' use SVG
go
checkpoint
go
use master
go
exec sp_dboption tempdb,trunc,true
go
use tempdb
go
checkpoint
go
