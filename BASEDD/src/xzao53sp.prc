/*E*/
/*Fichier : $Id: xzao53sp.prc,v 1.5 1999/02/26 11:30:13 gaborit Exp $      Release : $Revision: 1.5 $        Date : $Date: 1999/02/26 11:30:13 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao53sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	12 Sep 1994	: Creation
* volcic	20 Oct 1994	: Suppr XDC_ECHEC (v1.2)
* volcic	24 Oct 1994	: Ajout parametre va_Numero_in (v1.3)
* volcic	09 Nov 1994	: Ajout #ifdef CI (v1.4)
* C.T. 		22/01/98	: modif interface (1545) V1.5
* P.N.		02/12/98	: prise en compte de XDC_LIB_TYPEM_DATEX dem/1724 1.5
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao53sp.h" */

/* static char *version = "@(#)xzao53.c	1.1 23/18/94 : xzao53" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajout d'un District dans la table DISTRICT
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAO;53
   @va_Basedd_in        char(3),
   @va_Numero_in	tinyint,
   @va_NomDistrict_in	char(25),
   @va_Abreviation_in	char(2),
   @va_Type_in		char(6),
   @va_Serveur_in	char(7),
   @va_Machine_in	char(10),
   @va_Resultat_out	int output
   as 

/* 
*
* SP	XZAO53SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Nom		va_NomDistrict_in
* char(2)		va_Abreviation_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf		va_Resultat_out	
*
* VALEUR RENDUE :
*
* Ajoute un district a une base
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
* Insertion dans la table RES_DIS (DISTRICT)
*
* MECANISMES :
*
------------------------------------------------------*/
declare
   
   /*A
   ** Definition des variables globales
   */

   @vl_NumDistrict	tinyint,
   @vl_Type		tinyint,
   @vl_Numero		tinyint


   select @va_Resultat_out = XDC_LU
   
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_Basedd_in = null 
   or @va_Numero_in = null  
   or @va_NomDistrict_in = null  
   or @va_Abreviation_in = null
   or @va_Type_in = null
   or @va_Serveur_in = null
      
      return XDC_ARG_INV
   
   else   
   begin
      
      /*A
      **  Insertion du nom de l'autoroute argument dans la table RES_DIS de la base
      **  passee en argument, si celui ci n'existe pas
      */   
      
      if @va_Basedd_in = XDC_BASE_CFG
         select @vl_NumDistrict = numero from CFG..RES_DIS where nom = @va_NomDistrict_in

#ifdef CI

      else
         select @vl_NumDistrict = numero from CFT..RES_DIS where nom = @va_NomDistrict_in
      
#endif

      if @vl_NumDistrict = null
      begin
	 /* type du district */
	 if @va_Type_in = XDC_LIB_TYPEM_SD
		select @vl_Type = XDC_TYPEM_SD
	 else if @va_Type_in = XDC_LIB_TYPEM_SC
		select @vl_Type = XDC_TYPEM_SC
	 else if @va_Type_in = XDC_LIB_TYPEM_PC2
		select @vl_Type = XDC_TYPEM_PC2
	 else if @va_Type_in = XDC_LIB_TYPEM_PCS
		select @vl_Type = XDC_TYPEM_PCS
	 else if @va_Type_in = XDC_LIB_TYPEM_DATEX
		 select @vl_Type = 0

         if @va_Basedd_in = XDC_BASE_CFG
         begin   
            insert CFG..RES_DIS (numero, nom, code, type, serveur, machine)
            values ( @va_Numero_in, @va_NomDistrict_in, @va_Abreviation_in, @vl_Type, @va_Serveur_in, @va_Machine_in)
         end   

#ifdef CI

         else
         begin   
            insert CFT..RES_DIS (numero, nom, code, type, serveur, machine)
            values ( @va_Numero_in, @va_NomDistrict_in, @va_Abreviation_in, @vl_Type, @va_Serveur_in, @va_Machine_in)
         end 

#endif
         
         /*A
         ** Erreur d'insertion
         */
                     
         if @@rowcount <> 1
         begin
                     
            /*A
            ** Affectation du parametre de sortie Resultat a XDC_AJOUT
            */
         
            select @va_Resultat_out = XDC_AJOUT
                        
            return XDC_NOK
         end
         
         /*A
         ** Affectation du parametre de sortie Resultat a XDC_AJOUT
         */
         
         select @va_Resultat_out = XDC_AJOUT
         
      end
   end
      
   return XDC_OK
         
go  
