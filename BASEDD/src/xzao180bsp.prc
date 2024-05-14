/*E*/
/*Fichier : $Id: xzao180bsp.prc,v 1.1 1994/10/05 19:38:44 volcic Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/10/05 19:38:44 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao180sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao180sp.h" */

/* static char *version = "@(#)xzao180.c	1.1 20/09/94 : xzao180" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajout d'un capteur de tunnel dans la base
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAO;180
   @va_Basedd_in        	char(3),
   @va_NomCapteur_in		char(10),
   @va_Autoroute_in		char(4),
   @va_NomServeur_in		char(10),
   @va_PR_in			T_PR,
   @va_Sens_in			T_SENS,
   @va_Type_in			tinyint,
   @va_NumBit_in		tinyint,
   @va_Resultat_out		int output
   as 

/* 
*
* SP	XZAO180SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd		va_Basedd_in
* XDY_NomEqt		va_NomCapteur_in
* XDY_NomAuto		va_Autoroute_in
* XDY_PR		va_PR_in
* XDY_Sens		va_Sens_in
* XDY_Octet		va_Type_in
* XDY_Octet		va_NumBit_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf	   	va_Resultat_out	
*
* VALEUR RENDUE :
*
* Ajoute d'un capteur de tunnel en base
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
* Insertion ou modification dans la table EQT_CPT (CAPTEUR_TUNNEL)
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	19 Sep 1994	: Creation
* GGY		26/09/23	: Ajout district CA (DEM483)
------------------------------------------------------*/
declare
   
   /*A
   ** Definition des variables globales
   */

   @vl_NumCapteur	smallint,
   @vl_Numero		smallint
   
   
   select @va_Resultat_out = XDC_ECHEC
   
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_Basedd_in = null or @va_Basedd_in <> "CFG" or @va_Basedd_in <> "CFT"  
   or @va_NomCapteur_in = null or @va_Autoroute_in = null or @va_NomServeur_in = null
   or @va_PR_in = null or @va_Sens_in = null or @va_Type_in = null or @va_NumBit_in = null 
   
      return XDC_ARG_INV
   
   else   
   begin
      
	/* Verification du site */
	select  @vl_Site = numero
		from CFG..RES_DIS
		where serveur = @@servername and type = XDC_TYPEM_SD

		/* Si le site est CI ou CA */
		if @@rowcount = 1
		begin
      
      
         /*A
         **  Travail dans la base CFG
         */   
      
         if @va_Basedd_in = "CFG"
         begin
         
            /*A
            ** Verification de l'existence du capteur de tunnel comme equipement
            */
         
            select @vl_NumCapteur = numero from CFG..EQT_GEN
              where autoroute in
                     (select numero from  CFG..RES_AUT
                        where nom = @va_Autoroute_in)
                and type = XDC_EQT_CPT
                and PR   = @va_PR_in
                and sens = @va_Sens_in
                and nom  = @va_NomCapteur_in  
                
            /*A
            ** Le capteur existe dans EQT_GEN
            */
               
            if @vl_NumCapteur <> null
            begin
               
               select @vl_Numero = numero from CFG..EQT_CPT
                 where numero = @vl_NumCapteur
                
               /*A
               ** Le capteur n'existe pas dans EQT_CPT
               ** Insertion du tunnel dans la table EQT_CPT
               */
               
               if @vl_Numero = null
               begin
               
                  insert into CFG..EQT_CPT
                  values ( @vl_NumCapteur, @va_Type_in, @va_NumBit_in )
                  
                  /*A
                  ** Si erreur d'insertion
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
               
               else
                
               /*A
               ** Le demi-echangeur existe dans EQT_CPT
               ** Modification du tunnel dans la table EQT_CPT
               */
                     
               begin
               
                 update CFG..EQT_CPT
                  set type    = @va_Type_in,
                      num_bit = @va_NumBit_in
                  where numero = @vl_NumCapteur
               
                  /*A
                  ** Si erreur de modification
                  */
                     
                  if @@rowcount <> 1
                  begin
                     
                     /*A
                     ** Affectation du parametre de sortie Resultat a XDC_MODIF
                     */
         
                     select @va_Resultat_out = XDC_MODIF
                          
                     return XDC_NOK
                  end   
                  
                  /*A
                  ** Affectation du parametre de sortie Resultat a XDC_MODIF
                  */
         
                  select @va_Resultat_out = XDC_MODIF
                     
               end
            end
                  
            else
                 
            /*A
            ** Le capteur ou l'autoroute n'existe pas dans EQT_GEN et RES_AUT
            ** Affectation du parametre de sortie Resultat a XDC_ECHEC
            */
            
               select @va_Resultat_out = XDC_ECHEC
               
         end
      end
   end         
     
   return XDC_OK
         
go  
