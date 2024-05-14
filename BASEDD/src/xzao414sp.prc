/*E*/
/*  Fichier : $Id: xzao414sp.prc,v 1.4 2007/07/10 12:19:26 pc2dpdy Exp $      Release : $Revision: 1.4 $        Date : $Date: 2007/07/10 12:19:26 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao414sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale
*   ou reference au fichier xzao414sp.h >
*
------------------------------------------------------
* SEQUENCE D'APPEL : 
*
* SP	XZAO414SP
*
* PARAMETRES EN ENTREE :
*
* PARAMETRES EN SORTIE :	
* 	va_Nom_out		T_NOM 		: nom site
* 	va_Numero_out		T_SITE		: numero site
* 	va_Type_out		tinyint		: type pc
* 	va_Serveur_out		char(6)		: nom serveur SQL
* 	va_Machine_out		T_NOM_MACHINE 	: nom machine du serveur
*
*
* VALEUR RENDUE :
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
* rechercher les caracteristiques du site
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* Torregrossa	10 Dec 1997	: Creation
* Torregrossa	09 Fev 1998	: Ajout des disricts voisin (V1.2)
* Niepceron	26 Avr 2005	: retourne le nom au lieu du code dem462 1.3
* JMG	10/07/07	: 3 districts de recouvrement 1.4
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao414sp.h" */

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

/* static char *version = "@(#)xzao414sp.prc	1.1 12/10/97 : xzao414sp" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */

use PRC
go

drop procedure XZAO414
go

create procedure XZAO414
	@va_Site_in 		char(2),
	@va_Nom_out		T_NOM output,
	@va_Numero_out		T_SITE output,
	@va_Type_out		tinyint output,
	@va_Serveur_out		char(6) output,
	@va_Machine_out		T_NOM_MACHINE output,
	@va_district1_out	T_SITE output,
	@va_district2_out	T_SITE output,
	@va_district3_out	T_SITE output
as
   /* recherche les infos du site */
   select 
	@va_Numero_out = numero, 
	@va_Nom_out = nom, 
	@va_Type_out = type, 
	@va_Serveur_out = serveur, 
	@va_Machine_out = machine 
   from CFG..RES_DIS
   where code = @va_Site_in

   /* recherche des sites sur zones de recouvrement */
   select distinct
	district_recouvrement 
   into #DIS
   from CFG..RES_POR
   where district = @va_Numero_out and district_recouvrement != @va_Numero_out

   set rowcount 1
   select @va_district1_out = district_recouvrement
   from #DIS
   delete #DIS
   select @va_district2_out = district_recouvrement
   from #DIS
   delete #DIS
   select @va_district3_out = district_recouvrement
   from #DIS
   set rowcount 0

   return XDC_OK
         
go  
