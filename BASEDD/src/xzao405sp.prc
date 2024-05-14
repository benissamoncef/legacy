/*E*/
/*Fichier : $Id: xzao405sp.prc,v 1.5 2018/07/11 11:15:44 pc2dpdy Exp $      Release : $Revision: 1.5 $        Date : $Date: 2018/07/11 11:15:44 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao405sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
* Guilhou	05/06/97	dest TFM sur 50 car (dem/1440) 1.3
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao405sp.h" */

/* static char *version = "@(#)xzao405.c	1.1 20/09/94 : xzao405" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajout d'une destination temps de parcours dans la base
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAO;405
   @va_Basedd_in        	char(3),
   @va_NumDest_in		smallint,
   @va_Autoroute_in		char(4),
   @va_PR_in			T_PR,
   @va_Sens_in			T_SENS,
   @va_Libelle_PMV_in		char(15),
   @va_Libelle_TFM_in		char(50),
   @va_Type_loc_in		bit,
   @va_Inhibition_in		bit,
   @va_Resultat_out		int output
   as 

/* 
*
* SP	XZAO405SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd		va_Basedd_in
* XDY_Eqt		va_NumDest_in
* XDY_NomAuto		va_Autoroute_in
* XDY_PR		va_PR_in
* XDY_Sens		va_Sens_in
* XDY_Ligne_PMV		va_Libelle_PMV_in
* XDY_Nom		va_Libelle_TFM_in
* XDY_Booleen		va_Type_loc_in
* XDY_Booleen		va_Inhibition_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf	   	va_Resultat_out	
*
* VALEUR RENDUE :
*
* Ajout d'une destination de parcours en base
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
* Insertion ou modification dans la table RES_DTP 
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer	06 Fev 1996	: Creation (DEM/1395)
* LCL		11/07/18	: correction syntaxe from
* GGY		26/09/23	: Ajout district CA (DEM483)
* GGY		17/10/23	: Correction si CFG vide (DEM483)
------------------------------------------------------*/
declare
   
   /*A
   ** Definition des variables globales
   */

   @vl_Autoroute	tinyint,
   @vl_NumDest		smallint,
   @vl_Numero		smallint,
   @vl_Site			char(10)
   
   
   select @va_Resultat_out = XDC_ECHEC
   
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_Basedd_in = null  
   or @va_NumDest_in = null 
   or @va_Autoroute_in = null 
   or @va_PR_in = null 
   or @va_Sens_in = null 
   or @va_Libelle_PMV_in = null
   or @va_Libelle_TFM_in = null
   or @va_Type_loc_in = null
   or @va_Inhibition_in = null
   
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
            ** Verification de l'existence de l'autoroute comme equipement
            */
         
            select @vl_Autoroute = numero from CFG..RES_AUT
              where nom = @va_Autoroute_in
                        
            
            /*A
            ** L'autoroute existe
            */
                     
            if @vl_Autoroute <> null 
            begin
                
                select @vl_NumDest = numero from CFG..RES_DTP
                  where numero  = @va_NumDest_in  
                
               /*A
               ** La destination n'existe pas dans RES_DTP
               */
               
               if @vl_NumDest = null
               begin
               
                  /*A
                  ** Insertion de la destination dans la table RES_DTP
                  */

                  insert into CFG..RES_DTP
                  values ( @va_NumDest_in, @vl_Autoroute, @va_PR_in, @va_Sens_in, @va_Libelle_PMV_in, @va_Libelle_TFM_in, 
                  	@va_Type_loc_in, @va_Inhibition_in  )
                  
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
               ** L'equipement ZDP existe dans EQT_GEN
               */
                     
               begin
                  
                  /*A
                  ** Modification de la destination temps de parcours dans la table RES_DTP
                  */
                  
                  update CFG..RES_DTP
                  set autoroute 	= @vl_Autoroute, 
                      PR		= @va_PR_in, 
                      sens		= @va_Sens_in, 
                      libelle_PMV	= @va_Libelle_PMV_in, 
                      libelle_TFM	= @va_Libelle_TFM_in, 
                      type_localisation	= @va_Type_loc_in, 
                      inhibition	= @va_Inhibition_in
                  where numero = @vl_NumDest
               
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
            ** Le serveur ou l'autoroute n'existe pas dans EQT_GEN et RES_AUT
            ** Affectation du parametre de sortie Resultat a XDC_ECHEC
            */
            
               select @va_Resultat_out = XDC_ECHEC
               
         end

#ifdef CI
         
         else
               
         /*A
         **  Travail dans la base CFT
         */   
         begin
         
         
            /*A
            ** Verification de l'existence de l'autoroute comme equipement
            */
         
            select @vl_Autoroute = numero from CFT..RES_AUT
              where nom = @va_Autoroute_in
                        
            
            /*A
            ** L'autoroute existe
            */
                     
            if @vl_Autoroute <> null 
            begin
                
                select @vl_NumDest = numero from CFT..RES_DTP
                  where numero  = @va_NumDest_in  
                
               /*A
               ** La destination n'existe pas dans RES_DTP
               */
               
               if @vl_NumDest = null
               begin
               
                  /*A
                  ** Insertion de la destination dans la table RES_DTP
                  */

                  insert into CFT..RES_DTP
                  values ( @va_NumDest_in, @vl_Autoroute, @va_PR_in, @va_Sens_in, @va_Libelle_PMV_in, @va_Libelle_TFM_in, 
                  	@va_Type_loc_in, @va_Inhibition_in  )
                  
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
               ** L'equipement ZDP existe dans EQT_GEN
               */
                     
               begin
                  
                  /*A
                  ** Modification de la destination temps de parcours dans la table RES_DTP
                  */
                  
                  update CFT..RES_DTP
                  set autoroute 	= @vl_Autoroute, 
                      PR		= @va_PR_in, 
                      sens		= @va_Sens_in, 
                      libelle_PMV	= @va_Libelle_PMV_in, 
                      libelle_TFM	= @va_Libelle_TFM_in, 
                      type_localisation	= @va_Type_loc_in, 
                      inhibition	= @va_Inhibition_in
                  where numero = @vl_NumDest
               
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
            ** Le serveur ou l'autoroute n'existe pas dans EQT_GEN et RES_AUT
            ** Affectation du parametre de sortie Resultat a XDC_ECHEC
            */
            
               select @va_Resultat_out = XDC_ECHEC
               
         end
      
#endif
      
      end
   end         
     
   return XDC_OK
         
go  
