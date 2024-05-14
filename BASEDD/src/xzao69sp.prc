/*E*/
/*Fichier : $Id: xzao69sp.prc,v 1.5 2018/07/11 09:38:23 pc2dpdy Exp $      Release : $Revision: 1.5 $        Date : $Date: 2018/07/11 09:38:23 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao69sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao69sp.h" */

/* static char *version = "@(#)xzao69.c	1.1 23/18/94 : xzao69" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajout d'un point de mesure dans la table EQT_PNT_MES
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAO;69
   @va_Basedd_in        	char(3),
   @va_NomStation_in		char(10),
   @va_Sens_in			tinyint,
   @va_Boucle_in		tinyint,
   @va_BoucleInv_in		tinyint,
   @va_Voies_in			tinyint,
   @va_Poids_in			bit,
   @va_Creation_in		bit,
   @va_Resultat_out		int output
   as 

/* 
*
* SP	XZAO69SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd	va_Basedd_in        		
* XDY_NomEqt	va_NomStation_in
* XDY_Sens	va_Sens_in	
* XDY_Octet	va_Boucle_in
* XDY_Octet	va_BoucleInv_in
* XDY_Octet	va_Voies_in
* XDY_Booleen	va_Poids_in
* XDY_Booleen	va_Resultat_in				
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf	   va_Resultat_out	
*
* VALEUR RENDUE :
*
* Ajoute une station de comptage en base
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
* Insertion, modification ou suppression dans la table EQT_PNT_MES (POINT MESURE)
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	19 Sep 1994	: Creation
* volcic	29 Nov 1994	: Suppr XDC_ECHEC (v1.3)
* LCL		05/07/17	: Modif PRA
* LCL		11/07/18	: correction syntaxe from
* GGY		26/09/23	: Ajout district CA (DEM483)
* GGY		17/10/23	: Correction si CFG vide (DEM483)
------------------------------------------------------*/
declare
   
   /*A
   ** Definition des variables globales
   */

   @vl_NumPtMes		smallint,
   @vl_NumEqt		smallint,
   @vl_NumStation	smallint,
   @vl_Site			char(10)
   
   select @va_Resultat_out = XDC_ECHEC
            
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_Basedd_in = null 
   or @va_NomStation_in = null 
   or @va_Sens_in = null 
   or @va_Boucle_in = null 
   or @va_BoucleInv_in = null
      
      return XDC_NOK
   
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
         
         select @vl_NumEqt = numero from CFG..EQT_GEN
           where nom  = @va_NomStation_in
             and type = XDC_EQT_RAD
             
         /*A
         ** La station RADT existe comme equipement
         */
         
         if @vl_NumEqt <> NULL
         begin
            
            select @vl_NumStation = station from CFG..RES_CHA
              where station = @vl_NumEqt
                and sens    = @va_Sens_in
               
            /*A
            ** La station RADT existe dans la table RES_CHA
            */
               
            if @vl_NumStation <> NULL
            begin
            
               select @vl_NumPtMes = station from CFG..EQT_PNT_MES
                 where station = @vl_NumEqt
                   and sens    = @va_Sens_in
                   and boucle  = @va_Boucle_in
               
               if @vl_NumPtMes = NULL
               begin
               
                  /*A
                  ** Le point de mesure n'existe pas dans la table EQT_PNT_MES
                  */
               
                  if @va_Creation_in = XDC_VRAI
                  begin
               
                     /*A
                     ** Creation positionnee => Insertion dans la table du point de mesure
                     */
            
                     insert into CFG..EQT_PNT_MES
                     values ( @vl_NumStation, @va_Sens_in, @va_Boucle_in, @va_BoucleInv_in, @va_Voies_in, @va_Poids_in ) 
            
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
            
               begin
            
                  /*A
                  ** Le point de mesure existe pas dans la table EQT_PNT_MES
                  */
               
                  if @va_Creation_in = XDC_VRAI
                  begin
               
                     /*A
                     ** Creation positionnee => Modification dans la table du point de mesure
                     */
    
    	             update CFG..EQT_PNT_MES
    	             set boucle_inverse = @va_BoucleInv_in,
                         voie           = @va_Voies_in,
                         poids          = @va_Poids_in
                     where station = @vl_NumPtMes
                       and sens    = @va_Sens_in   
    		       and boucle  = @va_Boucle_in
                     
                     /*A
                     ** Erreur de modification
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
         
                  else
                 
                  begin
                    
                     /*A
                     ** Creation non positionnee => Suppression dans la table du point de mesure
                     */
                 
                     delete CFG..EQT_PNT_MES
    	             where station = @vl_NumPtMes
                       and sens    = @va_Sens_in
                       and boucle  = @va_Boucle_in  
                 
                     /*A
                     ** Erreur de suppression
                     */
                     
                     if @@rowcount <> 1
                     begin
                     
                        /*A
                        ** Affectation du parametre de sortie Resultat a XDC_SUPPR
                        */
         
                        select @va_Resultat_out = XDC_SUPPR
                        
                        return XDC_NOK
                     end
               
                     /*A
                     ** Affectation du parametre de sortie Resultat a XDC_SUPPR
                     */
         
                     select @va_Resultat_out = XDC_SUPPR
               
                  end
               end   
            end     
            else
                 
               /*A
               ** La station n'existe pas dans la table RES_CHA
               **
               ** Affectation du parametre de sortie Resultat a XDC_ECHEC
               */
         
               select @va_Resultat_out = XDC_ECHEC
           
         end
                 
         else
              
            /*A
            ** La station n'existe pas comme equipement
            **
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
         
         select @vl_NumEqt = numero from CFT..EQT_GEN
           where nom  = @va_NomStation_in
             and type = XDC_EQT_RAD
             
         /*A
         ** La station RADT existe comme equipement
         */
         
         if @vl_NumEqt <> NULL
         begin
            
            select @vl_NumStation = station from CFT..RES_CHA
              where station = @vl_NumEqt
                and sens    = @va_Sens_in
               
            /*A
            ** La station RADT existe dans la table RES_CHA
            */
               
            if @vl_NumStation <> NULL
            begin
            
               select @vl_NumPtMes = station from CFT..EQT_PNT_MES
                 where station = @vl_NumEqt
                   and sens    = @va_Sens_in
                   and boucle  = @va_Boucle_in
               
               if @vl_NumPtMes = NULL
               begin
               
                  /*A
                  ** Le point de mesure n'existe pas dans la table EQT_PNT_MES
                  */
               
                  if @va_Creation_in = XDC_VRAI
                  begin
               
                     /*A
                     ** Creation positionnee => Insertion dans la table du point de mesure
                     */
            
                     insert into CFT..EQT_PNT_MES
                     values ( @vl_NumStation, @va_Sens_in, @va_Boucle_in, @va_BoucleInv_in, @va_Voies_in, @va_Poids_in ) 
            
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
            
               begin
            
                  /*A
                  ** Le point de mesure existe pas dans la table EQT_PNT_MES
                  */
               
                  if @va_Creation_in = XDC_VRAI
                  begin
               
                     /*A
                     ** Creation positionnee => Modification dans la table du point de mesure
                     */
    
    	             update CFT..EQT_PNT_MES
    	             set boucle_inverse = @va_BoucleInv_in,
                         voie           = @va_Voies_in,
                         poids          = @va_Poids_in
                     where station = @vl_NumPtMes
                       and sens    = @va_Sens_in   
    		       and boucle  = @va_Boucle_in
                     
                     /*A
                     ** Erreur de modification
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
         
                  else
                 
                  begin
                    
                     /*A
                     ** Creation non positionnee => Suppression dans la table du point de mesure
                     */
                 
                     delete CFT..EQT_PNT_MES
    	             where station = @vl_NumPtMes
                       and sens    = @va_Sens_in
                       and boucle  = @va_Boucle_in  
                 
                     /*A
                     ** Erreur de suppression
                     */
                     
                     if @@rowcount <> 1
                     begin
                     
                        /*A
                        ** Affectation du parametre de sortie Resultat a XDC_SUPPR
                        */
         
                        select @va_Resultat_out = XDC_SUPPR
                        
                        return XDC_NOK
                     end
               
                     /*A
                     ** Affectation du parametre de sortie Resultat a XDC_SUPPR
                     */
         
                     select @va_Resultat_out = XDC_SUPPR
               
                  end
               end   
            end     
            else
                 
               /*A
               ** La station n'existe pas dans la table RES_CHA
               **
               ** Affectation du parametre de sortie Resultat a XDC_ECHEC
               */
         
               select @va_Resultat_out = XDC_ECHEC
           
         end
                 
         else
              
            /*A
            ** La station n'existe pas comme equipement
            **
            ** Affectation du parametre de sortie Resultat a XDC_ECHEC
            */
         
            select @va_Resultat_out = XDC_ECHEC
          
      end


#endif
   
   end
   
   else
   /*A
   ** Pas au CI
   */
      return XDC_NOK
   
   end   
      
   return XDC_OK
         
go  
