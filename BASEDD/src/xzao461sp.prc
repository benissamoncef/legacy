/*E*/
/* Fichier : $Id: xzao461sp.prc,v 1.3 2017/11/15 12:52:31 devgfi Exp $        Release : $Revision: 1.3 $        Date : $Date: 2017/11/15 12:52:31 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao461sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
* VR	10/11/11	Creation  1.0 (DEM/1014)
* VR	27/11/12	Modification lecture zones par nom au lieu d'Id 	1.1
* JPL	10/10/17	Remplacement du champ 'code' par 'code_zone' (pour DEM 1236)  1.3
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao461sp.h" */

/* static char *version = "@(#)xzao461.c	1.0 10/11/11 : xzao461" ; */

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

if exists (select 1 from sysobjects where name = 'XZAO461' and type = 'P')
	drop procedure XZAO461
go


create procedure XZAO461
    @va_Basedd_in        	char(3),
	@va_Autoroute_in		char(4),
	@va_PR_in				T_PR,
	@va_Sens_in				T_SENS,
	@va_Nom_Dest_in			char(16),
	@va_Type_Dest_in		tinyint,
	@va_Sens_Dest_in		T_SENS,
	@va_Inhibition_in 		bit,
	@va_Nom_Zdp_in			char(30),
	@va_droptable_in		tinyint,
	@va_Resultat_out		int output
as

/* 
*
* SP	XZAO461SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd			va_Basedd_in
* XDY_NomAuto			va_Autoroute_in
* XDY_PR				va_PR_in
* XDY_Sens				va_Sens_in
* XDY_Nom				@va_Nom_Dest_in		
* XDY_Octet				@va_Type_Dest_in
* XDY_Booleen 			va_Inhibition_in
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
* Insertion ou modification dans la table DTP_PMV 
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer	06 Fev 1996	: Creation (DEM/1395)
* MG		18/06/12	: modfication du numero de destination en nom destination et numero_ZDP en nom_ZDP
------------------------------------------------------*/
declare
   
   /*A
   ** Definition des variables globales
   */

   @vl_Autoroute	tinyint,
   @vl_Numero		smallint,
   @vl_NumZDP		smallint,
   @vl_NumDest		smallint
   
   
   select @va_Resultat_out = XDC_ECHEC
   
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */

	if @va_Basedd_in = null  
		or @va_Nom_Dest_in = null 
		or @va_Autoroute_in = null 
		or @va_PR_in = null 
		or @va_Sens_in = null 
		or @va_Type_Dest_in = null
		or @va_Inhibition_in = null
   
		return XDC_ARG_INV
   
	else   
	begin
      
		if @va_Nom_Zdp_in = null
			return XDC_OK

		/*A
		**  Travail dans la base CFG
		*/   
		if @va_Basedd_in = XDC_BASE_CFG
		begin
			/*A
			** on vide la table sur flage va_droptable_in
			*/
			if @va_droptable_in = 1
				delete from CFG..DTP_PMV

			/*A
			** Verification de l'existence de l'autoroute comme equipement
			*/
			select @vl_Autoroute = numero from CFG..RES_AUT
			where nom = @va_Autoroute_in

			select @vl_Numero = numero from CFG..EQT_GEN
			where autoroute = @vl_Autoroute and
			PR = @va_PR_in and
			sens = @va_Sens_in
			and type = XDC_EQT_PMV

			/*A
			** Le PMV existe
			*/
			
			if @vl_Numero <> null 
			begin
            
                /*A
				** Select du numero de ZDP dans la table EQT_ZDP
				*/
				select @vl_NumZDP = code_zone from CFG..EQT_ZDP where libelle = @va_Nom_Zdp_in
				
				/*A
				** Si pas trouve
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
				** Select du numero de destination dans la table RES_DTP
				*/
                
            	select @vl_NumDest = numero from CFG..RES_DTP where libelle_PMV = @va_Nom_Dest_in and sens = @va_Sens_Dest_in
						
				/*A
				** Si pas trouve
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
				** Insertion de la destination dans la table DTP_PMV
				*/
				insert into CFG..DTP_PMV(numero_PMV , numero_dest, type_dest, inhibition, numero_ZDP)
				values ( @vl_Numero, @vl_NumDest, @va_Type_Dest_in, @va_Inhibition_in, @vl_NumZDP )
              
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
				** Le numero de PMV n'existe pas dans DTP_PMV et RES_AUT
				** Affectation du parametre de sortie Resultat a XDC_ECHEC
				*/           
				select @va_Resultat_out = XDC_ECHEC    
		end
#ifdef CI
        else
		/*A
		**  Travail dans la base CFT
		*/   
		if @va_Basedd_in = XDC_BASE_CFT
		begin

			/*A
			** on vide la table si le flag est a 1
			*/
			if @va_droptable_in = 1
				delete from CFT..DTP_PMV
			/*A
			** Verification de l'existence de l'autoroute comme equipement
			*/
			
			select @vl_Autoroute = numero from CFT..RES_AUT
			  where nom = @va_Autoroute_in
			
			select @vl_Numero = numero from CFT..EQT_GEN
			where autoroute = @vl_Autoroute and
			PR = @va_PR_in and
			sens = @va_Sens_in
			and type = XDC_EQT_PMV


			/*A
			** Le pmv existe
			*/
			if @vl_Numero <>0
			begin

				/*A
				** Select du numero de ZDP dans la table EQT_ZDP
				*/

				select @vl_NumZDP = code_zone from CFT..EQT_ZDP where libelle = @va_Nom_Zdp_in
								
				/*A
				** Si pas trouve
				*/
				if @@rowcount<>1
				begin
	             
					/*A
					** Affectation du parametre de sortie Resultat a XDC_AJOUT
					*/
					select @va_Resultat_out = XDC_AJOUT
	                  
					return XDC_NOK
				end   
				
				/*A
				** Select du numero de destination dans la table RES_DTP
				*/
	        	select @vl_NumDest = numero from CFT..RES_DTP where libelle_PMV = @va_Nom_Dest_in and sens = @va_Sens_Dest_in
				/*A
				** Si pas trouve
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
				** Insertion de la destination dans la table DTP_PMV
				*/
	
				insert into CFT..DTP_PMV(numero_PMV , numero_dest, type_dest, inhibition, numero_ZDP)
				values ( @vl_Numero, @vl_NumDest, @va_Type_Dest_in, @va_Inhibition_in, @vl_NumZDP )

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
				** Le numero de PMV n'existe pas dans DTP_PMV et RES_AUT
				** Affectation du parametre de sortie Resultat a XDC_ECHEC
				*/           
				select @va_Resultat_out = XDC_ECHEC    
		end
#endif     
	end
	
	return XDC_OK
go  
