/*E*/
/* Fichier : $Id: xzao578sp.prc,v 1.2 2013/08/28 09:10:48 pc2dpdy Exp $        Release : $Revision: 1.2 $        Date : $Date: 2013/08/28 09:10:48 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzao578sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* VR		13/01/12 : Creation (DEM/1016)
* PNI		06/05/2013 : Correction insertion dans CFT 1.2
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao578sp.h" */

/* static char *version = "@(#)xzao578.c	1.1 23/18/94 : xzao578" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAO578' and type = 'P')
	drop procedure XZAO578
go

	create procedure XZAO578
	   @va_Basedd_in         	     char(3),
	   @va_NumeroCondition_in		 smallint,
	   @va_NumeroScen_in		     smallint,
	   @va_TypeHoraire_in			 smallint,
	   @va_Jour_in			 		 tinyint,
	   @va_HorodateDebut_in			 datetime,
	   @va_HorodateFin_in			 datetime,
	   @va_HeureDebut_in			 smallint,
	   @va_MinuteDebut_in			 smallint,
	   @va_HeureFin_in				 smallint,
	   @va_MinuteFin_in			 	 smallint,
	   @va_CondAlternative_in		 smallint,
	   @va_Seuil_in				  	 int,
	   @va_DonneeTrafic_in			 smallint,
	   @va_Operateur_in			 	 tinyint,
	   @va_Conjonction1_in			 smallint,
	   @va_Conjonction2_in			 smallint,
	   @va_Conjonction3_in			 smallint,
	   @va_Conjonction4_in			 smallint,	   
	   @va_Station_in				 smallint,
	   @va_Zone_in				smallint,
	   @va_TypeJour_in			smallint,
	   @va_Libelle_in			varchar(100),
	   @va_Sens_in			tinyint,
	   @va_ConditionSimple_in		tinyint
	   as 

	   
/* 
*
* SP    XZAO578SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd        va_Basedd_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf       va_Resultat_out    

* CODE RETOUR :
*
* 0    XDC_OK
* 1    XDC_NOK 
*
*
* FONCTION :
*
* Mise a jour des donnees dans la table ZON_REG
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* VR    13 Jan 2012    : Creation (DEM/1016)
------------------------------------------------------*/
declare @vl_numero smallint
   
  if @va_Basedd_in             = null
		return XDC_ARG_INV
       
  if @va_HorodateDebut_in = XDC_DATE_NULLE select @va_HorodateDebut_in = null
  if @va_HorodateFin_in = XDC_DATE_NULLE select @va_HorodateFin_in = null
	
  if @va_Basedd_in = XDC_BASE_CFG
  begin
              update CFG..REG_CON
              set    type_horaire = @va_TypeHoraire_in,
							   jour = @va_Jour_in,
							   horodate_debut = @va_HorodateDebut_in,
							   horodate_fin = @va_HorodateFin_in,
							   heure_debut = @va_HeureDebut_in,
							   minute_debut = @va_MinuteDebut_in,
							   heure_fin = @va_HeureFin_in,
							   minute_fin = @va_MinuteFin_in,
							   cond_alternative = @va_CondAlternative_in,
							   seuil = @va_Seuil_in,
							   donnee_trafic = @va_DonneeTrafic_in,
							   operateur = @va_Operateur_in,
							   scenario = @va_NumeroScen_in,
							   conjonction1 = @va_Conjonction1_in,
							   conjonction2 = @va_Conjonction2_in,
							   conjonction3 = @va_Conjonction3_in,
							   conjonction4 = @va_Conjonction4_in,							   
							   station = @va_Station_in   ,
							   sens = @va_Sens_in   ,
							   zone = @va_Zone_in,
							   type_jour = @va_TypeJour_in,
							   libelle = @va_Libelle_in,
							   condition_simple=@va_ConditionSimple_in
                where  numero = @va_NumeroCondition_in

		if @@rowcount <> 1
		begin
			select @vl_numero = max(numero) from CFG..REG_CON

			if @vl_numero = null
				select @vl_numero = 1
			else
				select @vl_numero = @vl_numero+1
			insert into CFG..REG_CON (numero,
						   type_horaire,
						   jour,
						   horodate_debut,
						   horodate_fin,
						   heure_debut,													   
						   minute_debut,
						   heure_fin,													   
						   minute_fin,
						   cond_alternative,													   
						   seuil,
						   donnee_trafic,
						   operateur,
						   scenario,
						   conjonction1,
						   conjonction2,
						   conjonction3,
						   conjonction4,	
						   station,sens,type_jour, zone, libelle,condition_simple)
			  values ( @vl_numero,
				   @va_TypeHoraire_in,
				   @va_Jour_in,
				   @va_HorodateDebut_in,
				   @va_HorodateFin_in,
				   @va_HeureDebut_in,
				   @va_MinuteDebut_in,
				   @va_HeureFin_in,
				   @va_MinuteFin_in,
				   @va_CondAlternative_in,
				   @va_Seuil_in,
				   @va_DonneeTrafic_in,
				   @va_Operateur_in,
				   @va_NumeroScen_in,
				   @va_Conjonction1_in,
				   @va_Conjonction2_in,
				   @va_Conjonction3_in,
				   @va_Conjonction4_in,
			   @va_Station_in  	,@va_Sens_in,	@va_TypeJour_in , @va_Zone_in, @va_Libelle_in,@va_ConditionSimple_in)	

			if @@rowcount <> 1
			begin
				return XDC_NOK
			end
		end
  end
#ifdef  CI 
            else
			begin
                        update CFT..REG_CON
                        set    type_horaire = @va_TypeHoraire_in,
							   jour = @va_Jour_in,
							   horodate_debut = @va_HorodateDebut_in,
							   horodate_fin = @va_HorodateFin_in,
							   heure_debut = @va_HeureDebut_in,
							   minute_debut = @va_MinuteDebut_in,
							   heure_fin = @va_HeureFin_in,
							   minute_fin = @va_MinuteFin_in,
							   cond_alternative = @va_CondAlternative_in,
							   seuil = @va_Seuil_in,
							   donnee_trafic = @va_DonneeTrafic_in,
							   operateur = @va_Operateur_in,
							   scenario = @va_NumeroScen_in,
							   conjonction1 = @va_Conjonction1_in,
							   conjonction2 = @va_Conjonction2_in,
							   conjonction3 = @va_Conjonction3_in,
							   conjonction4 = @va_Conjonction4_in,							   
							   station = @va_Station_in  , 
							   sens = @va_Sens_in,
							   type_jour = @va_TypeJour_in ,
							   libelle=@va_Libelle_in,
							   condition_simple=@va_ConditionSimple_in
                        where  numero = @va_NumeroCondition_in

			if @@rowcount <> 1
			begin
				select @vl_numero = max(numero) from CFT..REG_CON

				if @vl_numero = null
					select @vl_numero = 1
				else
					select @vl_numero = @vl_numero+1

				insert into CFT..REG_CON (numero,
					   type_horaire,
					   jour,
					   horodate_debut,
					   horodate_fin,
					   heure_debut,													   
					   minute_debut,
					   heure_fin,													   
					   minute_fin,
					   cond_alternative,													   
					   seuil,
					   donnee_trafic,
					   operateur,
					   scenario,
					   conjonction1,
					   conjonction2,
					   conjonction3,
					   conjonction4,	
					   station,sens,type_jour,zone		, libelle  , condition_simple )
				   values ( @vl_numero,
						   @va_TypeHoraire_in,
						   @va_Jour_in,
						   @va_HorodateDebut_in,
						   @va_HorodateFin_in,
						   @va_HeureDebut_in,
						   @va_MinuteDebut_in,
						   @va_HeureFin_in,
						   @va_MinuteFin_in,
						   @va_CondAlternative_in,
						   @va_Seuil_in,
						   @va_DonneeTrafic_in,
						   @va_Operateur_in,
						   @va_NumeroScen_in,
						   @va_Conjonction1_in,
						   @va_Conjonction2_in,
						   @va_Conjonction3_in,
						   @va_Conjonction4_in,
						   @va_Station_in, @va_Sens_in,@va_TypeJour_in, @va_Zone_in,
						    @va_Libelle_in, @va_ConditionSimple_in
				   )	
															
		if @@rowcount <> 1
		begin
			return XDC_NOK
		end  
	end
end
#endif
      
    return XDC_OK
         
go  
