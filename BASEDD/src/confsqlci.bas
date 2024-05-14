/*X  Fichier : $Id: confsqlci.bas,v 1.2 1996/08/21 18:34:31 vathelot Exp $      Release : $Revision: 1.2 $        Date : $Date: 1996/08/21 18:34:31 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER confsqlci.bas
------------------------------------------------------
* DESCRIPTION :
* 
* cree les devices pour le centre d'information
* cree le user applicatif MIGRAZUR pour le centre d'information
* cree les bases pour le centre d'information
* 
*
------------------------------------------------------
* HISTORIQUE :
*
* C.TORREGROSSA	21/03/95	: Creation
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
-- configuration des devices
-----------------------------------------------------
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
   go

   -- ajout de la base EXP
   print "ajout de la base EXP"
   go
   create database EXP
      on EXP_dev=50 -- 50 meg
      log on HIS_dev=100 --100 meg sur autre device
   go
   use EXP
   exec sp_changedbowner MIGRAZUR
   use master
   print "base EXP : OK"
   go
   
   -- ajout de la base CFG
   print "ajout de la base CFG"
   go
   create database CFG
      on HIS_dev=50 -- 50 meg
      log on EXP_dev=50 --50 meg sur autre device
   go
   use CFG
   exec sp_changedbowner MIGRAZUR
   use master
   print "base CFG : OK"
   go
   
   -- ajout de la base CFT
   print "ajout de la base CFT"
   go
   create database CFT
      on EXP_dev=50 -- 50 meg
      log on HIS_dev=50 --50 meg sur autre device
   go
   use CFT
   exec sp_changedbowner MIGRAZUR
   use master
   print "base CFT : OK"
   go
   
   -- ajout de la base HIS
   print "ajout de la base HIS"
   go
   create database HIS
      on HIS_dev=1000 -- 1000 meg
      log on EXP_dev=100 --100 meg sur autre device
   go
   use HIS
   exec sp_changedbowner MIGRAZUR
   use master
   print "base HIS : OK"
   go
   
   -- ajout de la base CI_RSSD
   print "ajout de la base CI_RSSD"
   go
   create database CI_RSSD
      on EXP_dev=10 -- 10 meg
      log on HIS_dev=10 --10 meg sur autre device
   go
   print "base CI_RSSD : OK"
   go
   
   -- ajout de la base PRC
   print "ajout de la base PRC"
   go
   create database PRC
      on EXP_dev=50 -- 50 meg
   go
   use PRC
   exec sp_changedbowner MIGRAZUR
   use master
   print "base PRC : OK"
   go

   -- ajout de la base SVG
   print "ajout de la base SVG"
   go
   create database SVG
      on EXP_dev=50 -- 50 meg
      log on HIS_dev=50 --50 meg sur autre device
   go
   use SVG
   exec sp_changedbowner MIGRAZUR
   use master
   print "base SVG : OK"
   go
   
   -- ajout de la base VUE
   create database VUE
	on EXP_dev=5 --5 meg
   go
   use VUE
   go
   exec sp_changedbowner MIGRAZUR
   use master
   print "base VUE : OK"
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
