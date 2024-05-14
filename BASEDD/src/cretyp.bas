/*X  Fichier : $Id: cretyp.bas,v 1.4 2019/01/07 15:29:00 pc2dpdy Exp $      Release : $Revision: 1.4 $        Date : $Date: 2019/01/07 15:29:00 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER cretyp.bas
------------------------------------------------------
* DESCRIPTION DE LA PROCEDURE :
* creation des types utilises pour les tables d'une base
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	09/08/94	: Creation
* C.T.	26/08/94	: Ajout de T_DISTRICT
* JMG	08/08/18	: ligne PMV passe a 18 suppression SAGA DEM1306 1.4
* ABK	13/08/23	: correction pour CNA (DEM483)
-----------------------------------------------------*/
use PRC
go

/* Creation des types, defauts pour une base */
if exists (select * from systypes where name = 'T_TEL_NUM')
	begin
	execute sp_droptype T_TEL_NUM
	end

if exists (select * from systypes where name = 'T_PR')
	begin
	execute sp_droptype T_PR
	end

if exists (select * from systypes where name = 'T_NOM')
	begin
	execute sp_droptype T_NOM
	end

if exists (select * from systypes where name = 'T_SITE')
	begin
	execute sp_droptype T_SITE
	end

if exists (select * from systypes where name = 'T_AUTOROUTE')
	begin
	execute sp_droptype T_AUTOROUTE
	end

if exists (select * from systypes where name = 'T_SENS')
	begin
	execute sp_droptype T_SENS
	end

if exists (select * from systypes where name = 'T_VOIE')
	begin
	execute sp_droptype T_VOIE
	end

if exists (select * from systypes where name = 'T_OPERATEUR')
	begin
	execute sp_droptype T_OPERATEUR
	end

if exists (select * from systypes where name = 'T_TEXTE')
	begin
	execute sp_droptype T_TEXTE
	end

if exists (select * from systypes where name = 'T_EQUIPEMENT')
	begin
	execute sp_droptype T_EQUIPEMENT
	end

if exists (select * from systypes where name = 'T_ABREV')
	begin
	execute sp_droptype T_ABREV
	end

if exists (select * from systypes where name = 'T_LIGNE_PMV')
	begin
	execute sp_droptype T_LIGNE_PMV
	end

if exists (select * from systypes where name = 'T_NOM_MACHINE')
	begin
	execute sp_droptype T_NOM_MACHINE
	end

if exists (select * from systypes where name = 'T_DISTRICT')
	begin
	execute sp_droptype T_DISTRICT
	end

execute sp_addtype T_TEL_NUM, 'char(15)'
go
execute sp_addtype T_PR, 'int'
go
execute sp_addtype T_NOM, 'char(25)'
go
execute sp_addtype T_SITE, 'TINYINT'
go
execute sp_addtype T_AUTOROUTE, 'TINYINT'
go
execute sp_addtype T_VOIE, 'TINYINT'
go
execute sp_addtype T_SENS, 'TINYINT'
go
execute sp_addtype T_OPERATEUR, 'smallint'
go
execute sp_addtype T_TEXTE, 'char(250)'
go
execute sp_addtype T_EQUIPEMENT, 'smallint'
go
execute sp_addtype T_LIGNE_PMV, 'char(18)'
go
execute sp_addtype T_ABREV, 'char(15)'
go
execute sp_addtype T_NOM_MACHINE, 'char(10)'
go
execute sp_addtype T_DISTRICT, 'tinyint'

go
