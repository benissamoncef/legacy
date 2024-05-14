/*E*/
/*Fichier : $Id: xzao51sp.prc,v 1.5 1999/02/26 11:30:15 gaborit Exp $      Release : $Revision: 1.5 $        Date : $Date: 1999/02/26 11:30:15 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao51sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------*/

/* fichiers inclus */

/* #include 'xzao51sp.h' */

/* static char *version = '@(#)xzao51.c	1.1 23/18/94 : xzao51' ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajout d'une Autoroute dans la table AUTOROUTE
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAO;51
   @va_Basedd_in        char(3),
   @va_NomAutoroute_in	char(4),
   @va_Inversee_in	smallint,
   @va_Escota_in	tinyint,
   @va_Resultat_out	int = null output
   as 

/* 
*
* SP	XZAO51SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd		va_Basedd_in
* XDY_NomAuto		va_NomAutoroute_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf		va_Resultat_out	
*
* VALEUR RENDUE :
*
* Ajout d'une Autoroute en base
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
* Insertion dans la table RES_AUT (AUTOROUTE)
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	12 Sep 1994	: Creation
* volcic	18 Oct 1994	: Modif definition Autoroute (v1.2)
* volcic	09 Nov 1994	: Ajout #ifdef CI (v1.3)
* torregrossa	09 Nov 1994	: Ajout sens de l'autoroute (A57) (v1.4)
* P.N.  	02/12/98        : ajout colonne escota 1.5 dem/1724
* LCL    10/10/23    : Adaptation CNA avec offset sur Id des autoroutes en fonction des instances Magistra DEM-483
------------------------------------------------------*/
declare
   
   /*A
   ** Definition des variables globales
   */
   
   @vl_Autoroute	tinyint,
   @vl_Numero		tinyint,
   @vl_SiteMaitre T_SITE

   select @va_Resultat_out = XDC_LU
   
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_Basedd_in = null or @va_NomAutoroute_in = null 
      return XDC_ARG_INV
   else   
   begin


      
      /*A
      **  Insertion du nom de l'autoroute argument dans la table RES_AUT de la base
      **  passee en argument, si celui ci n'existe pas
      */         
      if @va_Basedd_in = XDC_BASE_CFG
      begin
         select @vl_Autoroute = numero from CFG..RES_AUT where nom = @va_NomAutoroute_in
         /*A
         **  Recherche du site maitre pour définir l'offset par concessionnaire 
         */
         select @vl_SiteMaitre = numero from CFG..RES_DIS where type = XDC_TYPEM_SD
      end

#ifdef CI

      else
      begin
         select @vl_Autoroute = numero from CFT..RES_AUT where nom = @va_NomAutoroute_in
         /*A
         **  Recherche du site maitre pour définir l'offset par concessionnaire 
         */
         select @vl_SiteMaitre = numero from CFT..RES_DIS where type = XDC_TYPEM_SD
      end      
#endif

      /*A
      ** L'autoroute n'existe pas
      */
      
      if @vl_Autoroute = null
      begin
         if @va_Basedd_in = XDC_BASE_CFG
         begin   
            select @vl_Numero = max(numero) from CFG..RES_AUT
            
            if @vl_Numero = null
               if @vl_SiteMaitre = XDC_CI
                  select @vl_Numero = 0
               else if @vl_SiteMaitre = XDC_CA  
                  select @vl_Numero = 100
            
            insert into CFG..RES_AUT
            values ( @vl_Numero + 1, @va_NomAutoroute_in)
            insert into CFG..RES_AUT_INV
            values ( @vl_Numero + 1, @va_Inversee_in, @va_Escota_in)
         end   

#ifdef CI

         else
         begin   
            select @vl_Numero = max(numero) from CFT..RES_AUT
            
            if @vl_Numero = null
               if @vl_SiteMaitre = XDC_CI
                  select @vl_Numero = 0
               else if @vl_SiteMaitre = XDC_CA  
                  select @vl_Numero = 100
            
            insert into CFT..RES_AUT
            values ( @vl_Numero + 1, @va_NomAutoroute_in)
            insert into CFT..RES_AUT_INV
            values ( @vl_Numero + 1, @va_Inversee_in, @va_Escota_in)
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
