/*E*/
/* Fichier : $Id: xzao467sp.prc,v 1.1 2013/01/25 09:59:39 pc2dpdy Exp $        Release : $Revision: 1.1 $        Date : $Date: 2013/01/25 09:59:39 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao467sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
* VR   04/06/2012        : Creation (DEM/1014 PMA)
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao467sp.h" */

/* static char *version = "@(#)xzao467sp.prc	1.2 1.2 14/11/11: xzao467" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Liste des PMVA pour une machine
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAO467' and type = 'P')
	drop procedure XZAO467
go	

create procedure XZAO467
   as 

/* 
*
* SP	XZAO467SP
*
* PARAMETRES EN ENTREE :
*
* 			
*
* PARAMETRES EN SORTIE :
*
*
* 
* VALEUR RENDUE :
*
* Retourne la liste des PMA de la Machine avec leur destinations
* 
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK 
*
* CONDITIONS D'UTILISATION :
*
* FONCTION :
*
* Retourne les parametres numero de PMA, nom du ST, numero de liaison, adresse rgs
* numero de type de PMA
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* VR	04/06/12	: Creation (DEM/PMA)
------------------------------------------------------*/
	      
	create table #PMA_DEST
	(
		numero				smallint,
		num_dest_un_proche		smallint null,
		nom_dest_un_proche		char(15) null,
		num_dest_un_lointaine	smallint null,
		nom_dest_un_lointaine	char(15) null,
		num_dest_deux_proche		smallint null,
		nom_dest_deux_proche		char(15) null,	
		num_dest_deux_lointaine		smallint null,
		nom_dest_deux_lointaine		char(15) null	
	)

	/*A
	**  Recuperer toutes les association pma destination par type de destination
	*/
	insert into #PMA_DEST (numero, num_dest_un_proche, nom_dest_un_proche) 
	(
		select DTPPMA.numero_PMA, DTPPMA.numero_dest, RESDTP.libelle_PMV
		from CFG..DTP_PMA DTPPMA, CFG..RES_DTP RESDTP
		where DTPPMA.type_dest = XDC_DEST_UN_PROCHE and
		RESDTP.inhibition = 0 and
		DTPPMA.numero_PMA = RESDTP.numero
	)
	
	insert into #PMA_DEST (numero, num_dest_un_lointaine, nom_dest_un_lointaine ) 
	(
		select DTPPMA.numero_PMA, DTPPMA.numero_dest, RESDTP.libelle_PMV
		from CFG..DTP_PMA DTPPMA, CFG..RES_DTP RESDTP
		where DTPPMA.type_dest = XDC_DEST_UN_LOIN and
		RESDTP.inhibition = 0 and
		DTPPMA.numero_PMA = RESDTP.numero
	)

	insert into #PMA_DEST (numero, num_dest_deux_proche, nom_dest_deux_proche )  
	(
		select DTPPMA.numero_PMA, DTPPMA.numero_dest, RESDTP.libelle_PMV
		from CFG..DTP_PMA DTPPMA, CFG..RES_DTP RESDTP
		where DTPPMA.type_dest = XDC_DEST_DEUX_PROCHE and
		RESDTP.inhibition = 0 and
		DTPPMA.numero_PMA = RESDTP.numero
	)
	
	insert into #PMA_DEST (numero, num_dest_deux_lointaine, nom_dest_deux_lointaine )  
	(
		select DTPPMA.numero_PMA, DTPPMA.numero_dest, RESDTP.libelle_PMV
		from CFG..DTP_PMA DTPPMA, CFG..RES_DTP RESDTP
		where DTPPMA.type_dest = XDC_DEST_DEUX_LOIN and
		RESDTP.inhibition = 0 and
		DTPPMA.numero_PMA = RESDTP.numero
	)
	
	select numero, num_dest_un_proche, nom_dest_un_proche, num_dest_un_lointaine, nom_dest_un_lointaine, num_dest_deux_proche, nom_dest_deux_proche, num_dest_deux_lointaine, nom_dest_deux_lointaine
	from #PMA_DEST
	
	return XDC_OK	
	
go  
