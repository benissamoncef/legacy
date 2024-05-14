/*E*/
/*  Fichier : $Id: xzad01sp.prc,v 1.13 2020/01/21 16:48:04 gesconf Exp $      Release : $Revision: 1.13 $        Date : $Date: 2020/01/21 16:48:04 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzad01
01sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* stocke en base un franchissement de seuil
*
------------------------------------------------------
* HISTORIQUE :*
* P.N.	24/02/97	: Creation	(dem/tdp) (V 1.1)
* P.N.	28/08/97	: Insert sur site voisin si zone de recouvrement	(dem/1483) (V 1.3)
* P.N.	24/10/97	: Insert sur site voisin si zone de recouvrement	(dem/1483) (V 1.4)
* P.N.	14/05/97	: Reveil sur FMC op uniquement au CI	(dem/1661) (V 1.5)
* JPL	16/05/07	: Correction bug inversion cas meteo et travaux
* 			  Prise en compte fmc traitement chaussees et chaussee glissante (DEM 644,646) 1.6
* JPL	08/06/07	: Prise en compte Fmc Basculement pour Travaux (DEM 647) 1.7
* JMG	06/09/09	: Sectorisation DEM887 1.8
* JPL	27/08/10	: Correction requete pour inclure les Fmc Traitement chaussees et Chaussee glissante;
* 			     la selection par classe inclut les Fmc Zone de stockage (DEM 942) 1.9
* PNI	09/0817		: R�duction des r�veils (DEM1235) v1.11
* PNI	23/02/17	: correction de la reduction des reveils : ajout du franchissement dans ts les cas dans FRN_TDP (dem1235)v1.12
* PNI	27/11/19	: integration des reveil de remont�e de bouchon en bretelle (ARBEB) DEM SAE-105 v1.13
* LCL	27/09/23 : Adaptations CNA DEM-483	
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* stocke en base un franchissement de seuil
* 
* Sequence d'appel
* SP    XZAD01_Ajout_Franchissement
* 
* Arguments en entree
* XDY_Eqt               va_Numero_in    : no de zdp 
* XDY_Octet             va_Site_in      : site 
* XDY_Datetime          va_Horodate_in
* XDY_Entier            va_NoFMC_in
* XDY_Octet             va_CleFMC_in
* 
* Arguments en sortie
* 
* Code retour
* XDC_OK
* XDC_NOK       : insertion ou modification impossible
* XDC_ARG_INV   : parametres avec valeur non autorisee ou
*                 numero inexistante
* 
* Conditions d'utilisation
* 
* Fonction
* Insertion  / Mise a jour dans la table TDP_FRN
-------------------------------------------------------*/

use PRC
go

drop procedure XZAD01
go


create procedure XZAD01

        @va_Numero_in           smallint        = null, 
        @va_Site_in             tinyint         = null,
        @va_Type_in             tinyint         = null,
        @va_Horodate_in         datetime        = null
        
as
        declare  @vl_Retour int,
                @vl_reveil bit,
                @vl_num_init int,
                @vl_max_num int,
                @vl_fmc int,
                @vl_cle tinyint,
                @vl_type smallint,
                @vl_classe tinyint,
                @vl_fmc_q int,
                @vl_cle_q tinyint,
                @vl_PR  int,
                @vl_PRt int,
                @vl_PRdeb int,
                @vl_PRfin int,
                @vl_Sens tinyint,
                @vl_Texte char(200),
                @vl_Autoroute tinyint,
                @vl_status int, 
                @vl_poste  smallint,
                @vl_site_voisin tinyint
                
                
        select @vl_Retour = XDC_OK

        /*A controle parametres en entree */
        if @va_Type_in = null or 
           (@va_Type_in <> XDC_SEUIL_TDP_HAUT and 
           @va_Type_in <>  XDC_SEUIL_TDP_BAS) or 
           @va_Site_in = null or 
           @va_Horodate_in = null or
           @va_Horodate_in = XDC_DATE_NULLE  
                return XDC_ARG_INV

	/* verifie que l'on gere les franchissement pour cette zdp */
	
/*	if not exists (select zdp from CFG..FRN_LIS where zdp=@va_Numero_in) 
		return XDC_OK*/

        /*A Recherche des caracteristique de la zdp */
        select 
                @vl_PRdeb=PR,
                @vl_PRfin=PR_fin,
                @vl_Sens=sens ,
                @vl_Autoroute = autoroute
             from CFG..EQT_GEN EQT,CFG..EQT_ZDP ZDP
        where 
             EQT.numero = @va_Numero_in and
             EQT.type = XDC_EQT_ZDP and
             ZDP.numero = EQT.numero

	select @vl_reveil = XDC_FAUX

                /*A Suivant le type de franchissement -> insert ou update */
        if @va_Type_in = XDC_SEUIL_TDP_HAUT
        begin
                select @vl_max_num=count(*) from EXP..FRN_TDP
        
                if @vl_max_num <> 0
                begin
                        select 
                                @vl_max_num = max(numero) 
                        from    EXP..FRN_TDP
                end
        
                select @vl_num_init = @vl_max_num
                
                
                
                
                if exists (select zdp from CFG..FRN_LIS where zdp=@va_Numero_in) 
			select @vl_reveil = XDC_VRAI
                
                declare fmc2 cursor for
                select distinct
                        FMC.numero,
                        FMC.cle,
                        FMC.type,
			poste_enrichisseur
                from EXP..FMC_GEN FMC
                where (FMC.fin = null or FMC.fin > @va_Horodate_in) and
                 FMC.debut <= @va_Horodate_in  and
		 FMC.cle= @va_Site_in and 
                 FMC.type=XZAEC_FMC_PosteOperateur              
                
                open fmc2
        
                fetch fmc2 into @vl_fmc,
                                @vl_cle,
                                @vl_type,
				@vl_poste
        
        /*A Pour chaque fmc trouvEe : */

                while (@@sqlstatus = 0)
                begin
                       if not exists ( select * from EXP..FRN_TDP
                                        where   zdp=@va_Numero_in and
                                                site_zdp=@va_Site_in and
                                                no_fmc=@vl_fmc and
                                                cle_fmc=@vl_cle and
                                                fin=null )
                        begin
                
                                /*B incremente le dernier numero de franchissement */
                                select  @vl_max_num = @vl_max_num + 1
                                insert EXP..FRN_TDP (numero,zdp,site_zdp,no_fmc,cle_fmc,debut,fin)
                                values (@vl_max_num,@va_Numero_in,@va_Site_in,@vl_fmc,@vl_cle,@va_Horodate_in,null)
                         end       
                                if ( @vl_reveil = XDC_VRAI)
                                begin
                                        select 
                                        @vl_Texte="DEPASSEMENT SEUIL TDP SUR ZONE "+rtrim(ZDP.libelle)+" AUT. "+rtrim(AUT.nom)+
                                                " SENS "+convert(char(1),GEN.sens)+
                                                " ENTRE PR "+convert(char(7),convert(decimal(9,3),GEN.PR)/1000)+
                                                " ET "+convert(char(7),convert(decimal(9,3),ZDP.PR_fin)/1000)
                                        from CFG..EQT_GEN GEN,CFG..EQT_ZDP ZDP, CFG..RES_AUT AUT
                                        where GEN.numero=@va_Numero_in and 
                                              GEN.numero=ZDP.numero and
                                              GEN.type=XDC_EQT_ZDP and
                                              AUT.numero=GEN.autoroute
                        				
					insert EXP..FMC_REV
                              					( evenement, cle, machine_operateur, texte, sit )
                                        		values ( @vl_fmc, @vl_cle,  @vl_poste, @vl_Texte,  @vl_cle)
                                end
        		
                	/*A     passe au segment suivant */
                	fetch fmc2 into @vl_fmc,
                                	@vl_cle,
                                	@vl_type,
					@vl_poste
        
                end
                close fmc2
        end
        else
        begin
                if exists (select zdp from CFG..FRN_LIS where zdp=@va_Numero_in) 
			select @vl_reveil = XDC_VRAI
               declare fmc2 cursor for
                select distinct
                        FMC.numero,
                        FMC.cle,
                        FMC.type,
			poste_enrichisseur
                from EXP..FMC_GEN FMC
                where (FMC.fin = null or FMC.fin > @va_Horodate_in) and
                 FMC.debut <= @va_Horodate_in  and
		 FMC.cle= @va_Site_in and 
                 FMC.type=XZAEC_FMC_PosteOperateur              
                
                open fmc2
        
                fetch fmc2 into @vl_fmc,
                                @vl_cle,
                                @vl_type,
				@vl_poste
        
        /*A Pour chaque fmc trouvEe : */

                while (@@sqlstatus = 0)
                begin
                       if exists ( select * from EXP..FRN_TDP
                                        where   zdp=@va_Numero_in and
                                                site_zdp=@va_Site_in and
                                                no_fmc=@vl_fmc and
                                                cle_fmc=@vl_cle and
                                                fin=null )
                        begin
                
                                 if ( @vl_reveil = XDC_VRAI)
                                begin
                                        select 
                                        @vl_Texte="RETOUR A LA NORMALE TDP SUR ZONE "+rtrim(ZDP.libelle)+" AUT. "+rtrim(AUT.nom)+
                                                " SENS "+convert(char(1),GEN.sens)+
                                                " ENTRE PR "+convert(char(7),convert(decimal(9,3),GEN.PR)/1000)+
                                                " ET "+convert(char(7),convert(decimal(9,3),ZDP.PR_fin)/1000)
                                        from CFG..EQT_GEN GEN,CFG..EQT_ZDP ZDP, CFG..RES_AUT AUT
                                        where GEN.numero=@va_Numero_in and 
                                              GEN.numero=ZDP.numero and
                                              GEN.type=XDC_EQT_ZDP and
                                              AUT.numero=GEN.autoroute
                        				
					insert EXP..FMC_REV
                              					( evenement, cle, machine_operateur, texte, sit )
                                        		values ( @vl_fmc, @vl_cle,  @vl_poste, @vl_Texte,  @vl_cle)
                                end
        		end
                	/*A     passe au segment suivant */
                	fetch fmc2 into @vl_fmc,
                                	@vl_cle,
                                	@vl_type,
					@vl_poste
        
                end
                close fmc2

                update EXP..FRN_TDP set fin=@va_Horodate_in
                where zdp=@va_Numero_in and site_zdp=@va_Site_in and fin=null
        end

        return @vl_Retour
go
