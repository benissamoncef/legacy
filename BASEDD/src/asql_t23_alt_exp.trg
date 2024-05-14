/*E*/
/*Fichier : $Id: asql_t23_alt_exp.trg,v 1.6 2017/04/02 16:21:25 pc2dpdy Exp $      Release : $Revision: 1.6 $        Date : $Date: 2017/04/02 16:21:25 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAA * FICHIER xzaa01sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Trigger sur ALT_EXP
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.  09/01/98	: Creation (1532) V1.1
* JMG	24/03/11	; ajout alerte localisee V1.2 
* PNI	15/10/12	: ajout du site VD pour traitement alerte meteo PR DEM1055 v1.4
* PNI	04/09/13	: ajout du site CA pour traitement alerte meteo VE DEM1079 v1.5
*  JMG           03/03/17 : regio DEM1220
* LCL   26/09/23        : Adaptations CNA DEM-483
------------------------------------------------------*/

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  Avertir le CI d'une nouvelle alerte
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use EXP
go

drop trigger alt_exp_iu_trg
go

create trigger alt_exp_iu_trg
on ALT_EXP
for insert, update
as 

/* 
*
* SP	XZAA01SP
*
* PARAMETRES EN ENTREE :
*
* PARAMETRES EN SORTIE :
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
* MECANISMES :
*
------------------------------------------------------*/
declare

   /*A
   ** Declaration des variables locales
   */
    
   @vl_Horodate 	datetime,
   @vl_Type		tinyint,
   @vl_NumEqt		smallint,
   @vl_TypeEqt		tinyint,
   @vl_NomType		char(25),
   @vl_Numero		int,
   @vl_Cle		tinyint,
   @vl_Evt		int,
   @vl_Autoroute	tinyint,
   @vl_PR		int,
   @vl_Sens		tinyint,
   @vl_Creation		tinyint,
   @vl_SiteAlerte	tinyint,
   @vl_Qualif		tinyint,
   @vl_NomEqt		char(10),
   @vl_Texte		char(100),
   @vl_Portion          smallint,
   @vl_Translation      int,
   @vl_Autoroute_alerte tinyint,
   @vl_PR_alerte        int,
   @vl_Sens_alerte      tinyint

       
   
      /*A
      ** lire les infos de l'alerte
      */
      select
	@vl_Numero = numero, 
	@vl_TypeEqt = type_equipement, 
	@vl_NumEqt = equipement, 
	@vl_Type = type, 
	@vl_Horodate = horodate, 
	@vl_Texte = texte,
	@vl_Qualif = qualification,
	@vl_SiteAlerte = sit
      from inserted

	select @vl_Autoroute_alerte=NULL

      /*A
      ** L'equipement et le type d'alerte existent 
      */
      
      if @vl_NumEqt != null and @vl_Type != null
      begin
      	select @vl_NomType = nom 
	from CFG..TYP_ALT
        where numero = @vl_Type
          
	/* rechercher l'equipement */
        select @vl_NomEqt	= nom,
         	@vl_Autoroute 	= autoroute,
         	@vl_PR		= PR,
         	@vl_Sens	= sens
        from CFG..EQT_GEN
        where type   = @vl_TypeEqt and numero = @vl_NumEqt
         
	if @vl_Qualif = XDC_ALT_INCONNUE or @vl_Qualif = null
        	select @vl_Creation = XDC_VRAI
	else
        	select @vl_Creation = XDC_FAUX
         
        /*tenter la recup dans ALT_LOC*/
        select @vl_Autoroute_alerte=autoroute,
                @vl_PR_alerte=PR,
                @vl_Sens_alerte=sens
        from EXP..ALT_LOC
        where numero = @vl_Numero
        and sit = @vl_SiteAlerte

        if (@@servername = XDC_SQLCI)
		if @vl_Autoroute_alerte=NULL
                begin
               		exec TASRV_CI...AARP01 	@vl_Numero, 
            				@vl_Horodate, 
            				@vl_Type, 
            				@vl_NomType, 
            				@vl_TypeEqt, 
            				@vl_NumEqt, 
            				@vl_NomEqt, 
            				@vl_Texte,
            				@vl_Autoroute, 
            				@vl_PR, 
            				@vl_Sens, 
            				@vl_Creation,
            				@vl_SiteAlerte
		end
		else
		begin
                       select  @vl_Portion=numero,
                                @vl_Translation=10000*(@vl_PR_alerte-PR_debut)/(PR_fin-PR_debut)
                                from CFG..RES_POR
                                where   autoroute=@vl_Autoroute_alerte and
                                PR_debut<@vl_PR_alerte and
                                PR_fin>=@vl_PR_alerte


                        exec TASRV_CI...AARP11   @vl_Numero,
                                        @vl_Horodate,
                                        @vl_Type,
                                        @vl_NomType,
                                        @vl_TypeEqt,
                                        @vl_NumEqt,
                                        @vl_NomEqt,
                                        @vl_Texte,
                                        @vl_Autoroute_alerte,
                                        @vl_PR_alerte,
                                        @vl_Sens_alerte,
                                        @vl_Creation,
                                        @vl_SiteAlerte,
                                        @vl_Portion,
                                        @vl_Translation
		end
        if (@@servername = XDC_SQLDP) and (@vl_SiteAlerte != XDC_DP)
		if @vl_Autoroute_alerte=NULL
                begin
               		exec TASRV_DP...AARP01 	@vl_Numero, 
            				@vl_Horodate, 
            				@vl_Type, 
            				@vl_NomType, 
            				@vl_TypeEqt, 
            				@vl_NumEqt, 
            				@vl_NomEqt, 
            				@vl_Texte,
            				@vl_Autoroute, 
            				@vl_PR, 
            				@vl_Sens, 
            				@vl_Creation,
            				@vl_SiteAlerte
		end
		else
		begin
                       select  @vl_Portion=numero,
                                @vl_Translation=10000*(@vl_PR_alerte-PR_debut)/(PR_fin-PR_debut)
                                from CFG..RES_POR
                                where   autoroute=@vl_Autoroute_alerte and
                                PR_debut<@vl_PR_alerte and
                                PR_fin>=@vl_PR_alerte


                        exec TASRV_DP...AARP11   @vl_Numero,
                                        @vl_Horodate,
                                        @vl_Type,
                                        @vl_NomType,
                                        @vl_TypeEqt,
                                        @vl_NumEqt,
                                        @vl_NomEqt,
                                        @vl_Texte,
                                        @vl_Autoroute_alerte,
                                        @vl_PR_alerte,
                                        @vl_Sens_alerte,
                                        @vl_Creation,
                                        @vl_SiteAlerte,
                                        @vl_Portion,
                                        @vl_Translation
		end
        if (@@servername = XDC_SQLVC) and (@vl_SiteAlerte != XDC_VC)
		if @vl_Autoroute_alerte=NULL
                begin
               		exec TASRV_VC...AARP01 	@vl_Numero, 
            				@vl_Horodate, 
            				@vl_Type, 
            				@vl_NomType, 
            				@vl_TypeEqt, 
            				@vl_NumEqt, 
            				@vl_NomEqt, 
            				@vl_Texte,
            				@vl_Autoroute, 
            				@vl_PR, 
            				@vl_Sens, 
            				@vl_Creation,
            				@vl_SiteAlerte
		end
		else
		begin
                       select  @vl_Portion=numero,
                                @vl_Translation=10000*(@vl_PR_alerte-PR_debut)/(PR_fin-PR_debut)
                                from CFG..RES_POR
                                where   autoroute=@vl_Autoroute_alerte and
                                PR_debut<@vl_PR_alerte and
                                PR_fin>=@vl_PR_alerte


                        exec TASRV_VC...AARP11   @vl_Numero,
                                        @vl_Horodate,
                                        @vl_Type,
                                        @vl_NomType,
                                        @vl_TypeEqt,
                                        @vl_NumEqt,
                                        @vl_NomEqt,
                                        @vl_Texte,
                                        @vl_Autoroute_alerte,
                                        @vl_PR_alerte,
                                        @vl_Sens_alerte,
                                        @vl_Creation,
                                        @vl_SiteAlerte,
                                        @vl_Portion,
                                        @vl_Translation
                end
        if (@@servername = XDC_SQLCA) and (@vl_SiteAlerte != XDC_CA)
		if @vl_Autoroute_alerte=NULL
                begin
               		exec TASRV_CA...AARP01 	@vl_Numero, 
            				@vl_Horodate, 
            				@vl_Type, 
            				@vl_NomType, 
            				@vl_TypeEqt, 
            				@vl_NumEqt, 
            				@vl_NomEqt, 
            				@vl_Texte,
            				@vl_Autoroute, 
            				@vl_PR, 
            				@vl_Sens, 
            				@vl_Creation,
            				@vl_SiteAlerte
		end
		else
		begin
                       select  @vl_Portion=numero,
                                @vl_Translation=10000*(@vl_PR_alerte-PR_debut)/(PR_fin-PR_debut)
                                from CFG..RES_POR
                                where   autoroute=@vl_Autoroute_alerte and
                                PR_debut<@vl_PR_alerte and
                                PR_fin>=@vl_PR_alerte


                        exec TASRV_CA...AARP11   @vl_Numero,
                                        @vl_Horodate,
                                        @vl_Type,
                                        @vl_NomType,
                                        @vl_TypeEqt,
                                        @vl_NumEqt,
                                        @vl_NomEqt,
                                        @vl_Texte,
                                        @vl_Autoroute_alerte,
                                        @vl_PR_alerte,
                                        @vl_Sens_alerte,
                                        @vl_Creation,
                                        @vl_SiteAlerte,
                                        @vl_Portion,
                                        @vl_Translation
		end
      end

go
