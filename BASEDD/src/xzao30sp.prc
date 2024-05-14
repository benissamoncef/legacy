/*E*/
/*  Fichier : $Id: xzao30sp.prc,v 1.8 2016/07/11 18:05:37 devgfi Exp $      Release : $Revision: 1.8 $        Date : $Date: 2016/07/11 18:05:37 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao30sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale
*   ou reference au fichier xzao30sp.h >
*
------------------------------------------------------
* SEQUENCE D'APPEL : 
*
* SP	XZAO30SP
*
* PARAMETRES EN ENTREE :
*
* PARAMETRES EN SORTIE :	
*
* XZAOT_District	Liste
*
* VALEUR RENDUE :
*
* Retourne la liste des districts
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
* Retour des principales colonnes de la table RES_DIS (DISTRICT)
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	25 Aug 1994	: Creation
* Torregrossa	10 Dec 1997	: maintien des param de sortie suite a ajout de colonne (1.2)
* niepceron	02 Dec 1998	: retourne uniquement les districts ayant un code != XX 1.3 dem/1724
* JPL		20/06/2016	: Degroupee ; retour des sites dans l'ordre de leur nom  1.7
* JPL		11/07/2016	: Ajout du parametre de base de donnees, conformement a l'utilisation  1.8
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao30sp.h" */

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

/* declaration de fonctions internes */

/* definition de fonctions externes */

use PRC
go

if exists (select 1 from sysobjects where name = 'XZAO30' and type = 'P')
	drop procedure XZAO30
go


create procedure XZAO30
	@va_basedd_in	char(3)
as

   select numero, nom, code from CFG..RES_DIS
   where type != 0
   order by nom

   return XDC_OK

go
