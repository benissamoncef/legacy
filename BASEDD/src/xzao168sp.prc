/*E*/
/*Fichier : $Id: xzao168sp.prc,v 1.6 2018/07/11 10:27:07 pc2dpdy Exp $      Release : $Revision: 1.6 $        Date : $Date: 2018/07/11 10:27:07 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao168sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao168sp.h" */

/* static char *version = "@(#)xzao168.c	1.1 20/09/94 : xzao168" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajout d'une sequence tube dans la base
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAO;168
   @va_Basedd_in        	char(3),
   @va_NomSequence_in		T_NOM,
   @va_Creation_in		bit,
   @va_Resultat_out		int output
   as 

/* 
*
* SP	XZAO168SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd		va_Basedd_in
* XDY_Nom		va_NomSequence_in
* XDY_Booleen		va_Creation_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf	   va_Resultat_out	
*
* VALEUR RENDUE :
*
* Ajoute une sequence tube en base
* 
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK 
*
* CONDITIONS D'UTILISATION :
*
* Config OFF LINE
*
* FONCTION :
*
* Insertion ou modification dans la table EQT_SEQ_TUB (EQUIPEMENT_SEQ_TUB)
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	19 Sep 1994	: Creation
* volcic	21 Oct 1994	: Suppr XDC_ECHEC et Ajout #ifdef CI (v1.2)
* volcic	09 Nov 1994	: Ajout #endif (v1.3)
* volcic	25 Nov 1994	: Suppr XDC_ECHEC (v1.4)
* LCL		11/07/18	: correction syntaxe from
* GGY		26/09/23	: Ajout district CA (DEM483)
* GGY		17/10/23	: Correction si CFG vide (DEM483)
------------------------------------------------------*/
declare
   
   /*A
   ** Definition des variables globales
   */

   @vl_NumTUB	tinyint,
   @vl_Numero	tinyint,
   @vl_Site		char(10)
   
   
   select @va_Resultat_out = XDC_LU
   
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_Basedd_in = null  
   or @va_NomSequence_in = null 
   or @va_Creation_in = null 
   
      return XDC_ARG_INV
   
   else   
   begin
      
		/* Verification que le site est bien celui du CI ou CA */
		select  @vl_Site = @@servername
	   
		/* Si le site est CI ou CA */
		if @vl_Site=XDC_SQLCI or @vl_Site=XDC_SQLCA
		begin
      
         /*A
         **  Travail dans la base CFG
         */   
      
         if @va_Basedd_in = XDC_BASE_CFG
         begin
         
            /*A
            ** Verification de l'existence de la sequence tube dans EQT_SEQ_TUB
            */
         
            select @vl_NumTUB = numero from CFG..EQT_SEQ_TUB
              where nom = @va_NomSequence_in
                
            /*A
            ** Le sequence tube n'existe pas dans EQT_SEQ_TUB
            */
               
            if @vl_NumTUB = null
            begin
                  
               if @va_Creation_in = XDC_VRAI
               begin
               
                  /*A
                  ** Creation positionne =>
                  ** Insertion du sequence tube dans la table EQT_SEQ_TUB
                  */
               
                  select @vl_Numero = max(numero) from CFG..EQT_SEQ_TUB
                  
                  if @vl_Numero = null
                     select @vl_Numero = 0
                        
                  insert into CFG..EQT_SEQ_TUB
                  values ( @vl_Numero + 1, @va_NomSequence_in, XDC_FAUX )
                  
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
                  
            else
           
            /*A
            ** Le sequence tube existe deja dans EQT_SEQ_TUB 
            */
         
            begin
            
               /*A
               ** Creation non positionne =>
               ** Marquage de la suppression de la sequence tube dans EQT_SEQ_TUB 
               */
               
               if @va_Creation_in = XDC_FAUX
               begin
                  
                  update CFG..EQT_SEQ_TUB
                  set supprime = XDC_VRAI
                  where numero = @vl_NumTUB
                    and nom    = @va_NomSequence_in
               end
               
            end      
         end

#ifdef CI

	 else
	 
	  /*A
         **  Travail dans la base CFT
         */   
      
         begin
         
            /*A
            ** Verification de l'existence de la sequence tube dans EQT_SEQ_TUB
            */
         
            select @vl_NumTUB = numero from CFT..EQT_SEQ_TUB
              where nom = @va_NomSequence_in
                
            /*A
            ** Le sequence tube n'existe pas dans EQT_SEQ_TUB
            */
               
            if @vl_NumTUB = null
            begin
                  
               if @va_Creation_in = XDC_VRAI
               begin
               
                  /*A
                  ** Creation positionne =>
                  ** Insertion du sequence tube dans la table EQT_SEQ_TUB
                  */
               
                  select @vl_Numero = max(numero) from CFT..EQT_SEQ_TUB
                  
                  if @vl_Numero = null
                     select @vl_Numero = 0
                        
                  insert into CFT..EQT_SEQ_TUB
                  values ( @vl_Numero + 1, @va_NomSequence_in, XDC_FAUX )
                  
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
                  
            else
           
            /*A
            ** Le sequence tube existe deja dans EQT_SEQ_TUB 
            */
         
            begin
            
               /*A
               ** Creation non positionne =>
               ** Marquage de la suppression de la sequence tube dans EQT_SEQ_TUB 
               */
               
               if @va_Creation_in = XDC_FAUX
               begin
                  
                  update CFT..EQT_SEQ_TUB
                  set supprime = XDC_VRAI
                  where numero = @vl_NumTUB
                    and nom    = @va_NomSequence_in
               end
               
            end      
         end     
      
#endif
      
      end
   end         
     
   return XDC_OK
         
go  
