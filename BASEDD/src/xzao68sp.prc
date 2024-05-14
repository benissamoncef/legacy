/*E*/
/*Fichier : $Id: xzao68sp.prc,v 1.5 2018/07/11 09:37:40 pc2dpdy Exp $      Release : $Revision: 1.5 $        Date : $Date: 2018/07/11 09:37:40 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao68sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao68sp.h" */

/* static char *version = "@(#)xzao68.c	1.1 23/18/94 : xzao68" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajout d'une station RADT dans la table EQT_CHA
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAO;68
   @va_Basedd_in        	char(3),
   @va_NomStation_in		char(10),
   @va_Sens_in			tinyint,
   @va_PR_in			int,
   @va_Resultat_out		int output
   as 

/* 
*
* SP	XZAO68SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd	va_Basedd_in        		
* XDY_NomEqt	va_NomStation_in
* XDY_Sens	va_Sens_in
* XDY_PR	va_PR_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf	   va_Resultat_out	
*
* VALEUR RENDUE :
*
* Ajoute une chaussee en base
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
* Insertion ou modification dans la table RES_CHA (CHAUSSEE)
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	16 Sep 1994	: Creation
* volcic	21 Oct 1994	: Suppr XDC_ECHEC et Ajout #ifdef CI (v1.2)
* C.T.		18 Fev 1998	: modif test sur rowcount en update suite a SQL 11.0.2.3 (1.3)
* LCL		05/07/17	: Modif PRA
* LCL 		11/07/18	: correction syntaxe from
* GGY		26/09/23	:	Ajout district CA (DEM483)
* GGY		17/10/23	: Correction si CFG vide (DEM483)
------------------------------------------------------*/
declare
   
   /*A
   ** Definition des variables globales
   */

   @vl_NumStation	smallint,
   @vl_NumCha		smallint,
   @vl_Site			char(10)
   
   select @va_Resultat_out = XDC_LU
            
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_Basedd_in = null
   or @va_NomStation_in = null 
   or @va_PR_in = null 
   or @va_Sens_in = null
   
      return XDC_ARG_INV
   
   else   
   begin

		/* Verification que le site est bien celui du CI ou CA */
		select  @vl_Site = @@servername
	   
		/* Si le site est CI ou CA */
		if @vl_Site=XDC_SQLCI or @vl_Site=XDC_SQLCA
		begin tran
      
      /*A
      **  Travail dans la base CFG
      */   
      
      if @va_Basedd_in = XDC_BASE_CFG
      begin
         
           select @vl_NumStation = numero from CFG..EQT_GEN
             where nom         = @va_NomStation_in
               and type        = XDC_EQT_RAD
           
           /*A
           ** La station RADT existe comme equipement
           */
         
           if @vl_NumStation <> NULL
           begin
              
              select @vl_NumCha = station from CFG..RES_CHA
               where station = @vl_NumStation
                 and sens    = @va_Sens_in
           
                  
              /*A
              ** La chausse existe
              */
              
              if @vl_NumCha <> null
              begin    
                  
                  /*A
                  ** Modification dans la table RES_CHA de la chaussee
                  */
    
    	          update CFG..RES_CHA
    	            set PR = @va_PR_in
                  where station = @vl_NumStation 
                    and sens    = @va_Sens_in
                    
                  /*A
                  ** Si Erreur de modification
                  */
                     
                  if @@rowcount > 1
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
               
               /*A
               ** La chaussee n'existe pas
               */
         
               begin   
                  
                  /*A
                  ** Insertion dans la table RES_CHA de la chaussee
                  */
    
    	          insert into CFG..RES_CHA
    	          values ( @vl_NumStation, @va_Sens_in, @va_PR_in )
                  
                  /*A
                  ** Si Erreur d'insertion
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
            ** Le station RADT n'existe pas comme equipement
            **
            ** Affectation du parametre de sortie Resultat a XDC_ECHEC
            */
         
               select @va_Resultat_out = XDC_ECHEC
         
      end
      
#ifdef CI      
         
      else
         
      /*A
      **  Travail dans la base CFG
      */   
      
     begin
         
           select @vl_NumStation = numero from CFT..EQT_GEN
             where nom         = @va_NomStation_in
               and type        = XDC_EQT_RAD
           
           /*A
           ** La station RADT existe comme equipement
           */
         
           if @vl_NumStation <> NULL
           begin
              
              select @vl_NumCha = station from CFT..RES_CHA
               where station = @vl_NumStation
                 and sens    = @va_Sens_in
           
                  
              /*A
              ** La chausse existe
              */
              
              if @vl_NumCha <> null
              begin    
                  
                  /*A
                  ** Modification dans la table RES_CHA de la chaussee
                  */
    
    	          update CFT..RES_CHA
    	            set PR = @va_PR_in
                  where station = @vl_NumStation 
                    and sens    = @va_Sens_in
                    
                  /*A
                  ** Si Erreur de modification
                  */
                     
                  if @@rowcount > 1
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
               
               /*A
               ** La chaussee n'existe pas
               */
         
               begin   
                  
                  /*A
                  ** Insertion dans la table RES_CHA de la chaussee
                  */
    
    	          insert into CFT..RES_CHA
    	          values ( @vl_NumStation, @va_Sens_in, @va_PR_in )
                  
                  /*A
                  ** Si Erreur d'insertion
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
            ** Le station RADT n'existe pas comme equipement
            **
            ** Affectation du parametre de sortie Resultat a XDC_ECHEC
            */
         
               select @va_Resultat_out = XDC_ECHEC
         
      end      
      
      
#endif
      
      commit tran
   end
      
   return XDC_OK
         
go  
