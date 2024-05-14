/*E*/
/*  Fichier : $Id: xzad10sp.prc,v 1.8 2018/01/09 08:15:48 gesconf Exp $      Release : $Revision: 1.8 $        Date : $Date: 2018/01/09 08:15:48 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzad10sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* stocke en base un franchissement de seuil du site voisin
*
------------------------------------------------------
* HISTORIQUE :
*
* P.N.	28/08/97	: Creation	(dem/1483) (V 1.2)
* P.N.	28/08/97	: Creation	(dem/1483) (V 1.3)
* P.N.	14/05/97	: Reveil sur FMC op uniquement au CI	(dem/1661) (V 1.4)
* JPL	16/05/07	: Correction bug inversion cas meteo et travaux
* 			  Prise en compte fmc traitement chaussees et chaussee glissante (DEM 644,646) 1.5
* JPL	08/06/07	: Prise en compte Fmc Basculement pour Travaux (DEM 647) 1.6
* JPL	27/08/10	: Correction requete pour inclure les Fmc Traitement chaussees et Chaussee glissante;
* 			     la selection par classe inclut les Fmc Zone de stockage (DEM 942) 1.7
* PNI 	15/12/17         : Ajout heure dans libelle reveil (DEM1235) v1.8
* ABK	26/09/23        : Ajout district CA (DEM-483)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* stocke en base un franchissement de seuil du site voisin
* 
* Sequence d'appel
* SP    XZAD10_Ajout_Franchissement
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

drop procedure XZAD10
go


create procedure XZAD10

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
                @vl_poste  smallint,
                @vl_Autoroute tinyint,
                @vl_siteCI tinyint
                
                


        select @vl_Retour = XDC_OK

        /*A controle parametres en entree */
        if @va_Type_in = null or 
           (@va_Type_in <> XDC_SEUIL_TDP_HAUT and 
           @va_Type_in <>  XDC_SEUIL_TDP_BAS) or 
           @va_Site_in = null or 
           @va_Horodate_in = null or
           @va_Horodate_in = XDC_DATE_NULLE  
                return XDC_ARG_INV

        /* initialisation variable locale */
        select @vl_siteCI = numero from CFG..RES_DIS where type = XDC_TYPEM_SD

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
                
                
                /*B On recherche d'abord les fmc non operateur , puis les operateurs */
                /*      Tres important pour le trigger  */
                declare fmc cursor for
                select distinct
                        FMC.numero,
                        FMC.cle,
                        FMC.type,
                        TYP.classe,
                        HIS.PR
                from EXP..FMC_GEN FMC, EXP..FMC_HIS HIS, EXP..FMC_TRT TRT, CFG..TYP_FMC TYP
                where (FMC.fin = null or FMC.fin > @va_Horodate_in) and
                      FMC.debut <= @va_Horodate_in  and

                      TRT.evenement = FMC.numero and
                      TRT.cle = FMC.cle and
                      TRT.sit = @va_Site_in and

                      TYP.numero = FMC.type and

                      HIS.numero = FMC.numero and
                      HIS.cle = FMC.cle and
                      HIS.autoroute = @vl_Autoroute and
                      (HIS.sens = @vl_Sens or HIS.sens = XDC_SENS_INCONNU) and
                      ( (HIS.PR >= @vl_PRdeb and HIS.PR <= @vl_PRfin) or 
                        TYP.classe in (XZAEC_ClasseMeteo,
                                       XZAEC_ClasseTravaux,
                                       XZAEC_ClasseBasculTrav) ) and
                      HIS.horodate_validation in ( select max(horodate_validation)
                                                   from EXP..FMC_HIS
                                                   where numero = FMC.numero and
                                                         cle = FMC.cle )

                order by type

                open fmc
        
                fetch fmc into  @vl_fmc,
                                @vl_cle,
                                @vl_type,
                                @vl_classe,
                                @vl_PR
        
        /*A Pour chaque fmc trouvEe : */

                while (@@sqlstatus = 0)
                begin
                
                /*A Si on a la tete du bouchon, insertion de la queue si ce n'est deja fait */
                if @vl_type = XZAEC_FMC_TeteBouchon
                begin
                        select @vl_fmc_q=numero,
                               @vl_cle_q=cle
                        from   EXP..FMC_GEN
                        where   cause=@vl_fmc and
                                cle_cause=@vl_cle

                        if not exists ( select * from EXP..FRN_TDP 
                                        where   zdp=@va_Numero_in and
                                                site_zdp=@va_Site_in and
                                                no_fmc=@vl_fmc_q and
                                                cle_fmc=@vl_cle_q and 
                                                debut=@va_Horodate_in )
                        begin
                                /*B incremente me dernier numero de franchissement */
                                select  @vl_max_num = @vl_max_num + 1
                                insert EXP..FRN_TDP (numero,zdp,site_zdp,no_fmc,cle_fmc,debut,fin)
                                values (@vl_max_num,@va_Numero_in,@va_Site_in,@vl_fmc_q,@vl_cle_q,@va_Horodate_in,null)
                        end
                end
                else
                /*A Pour un evenement de classe Meteo, */
                /*A  insertion uniquement si PR de queue ou de tete dans la zdp */
                if (@vl_classe = XZAEC_ClasseMeteo)
                begin
                        select 
                                @vl_PRt=PR
                        from EXP..FMC_MET
                        where   
                             numero =  @vl_fmc and
                             cle =  @vl_cle and
                             horodate in ( select max(horodate)
                                                 from EXP..FMC_MET
                                                 where numero =  @vl_fmc and
                                                        cle =  @vl_cle) 
                                                        
                        if ((@vl_PR >= @vl_PRdeb and @vl_PR <= @vl_PRfin) or 
                            (@vl_PRt >= @vl_PRdeb and @vl_PRt <= @vl_PRfin))
                        begin
                                /*B incremente me dernier numero de franchissement */
                                select  @vl_max_num = @vl_max_num + 1
                                insert EXP..FRN_TDP (numero,zdp,site_zdp,no_fmc,cle_fmc,debut,fin)
                                values (@vl_max_num,@va_Numero_in,@va_Site_in,@vl_fmc,@vl_cle,@va_Horodate_in,null)
                        end
                end
                else
                /*A Pour un evenement de classe Travaux, */
                /*A  insertion uniquement si PR de queue ou de tete dans la zdp */
                if @vl_classe in ( XZAEC_ClasseTravaux,
                                   XZAEC_ClasseBasculTrav )
                begin
                        select 
                                @vl_PRt=localisation_tete
                        from EXP..FMC_TRH
                        where   
                             numero =  @vl_fmc and
                             cle =  @vl_cle and
                             horodate in ( select max(horodate)
                                                 from EXP..FMC_TRH
                                                 where numero =  @vl_fmc and
                                                        cle =  @vl_cle) 
                                                        
                        if ((@vl_PR >= @vl_PRdeb and @vl_PR <= @vl_PRfin) or 
                            (@vl_PRt >= @vl_PRdeb and @vl_PRt <= @vl_PRfin))
                        begin
                                /*B incremente me dernier numero de franchissement */
                                select  @vl_max_num = @vl_max_num + 1
                                insert EXP..FRN_TDP (numero,zdp,site_zdp,no_fmc,cle_fmc,debut,fin)
                                values (@vl_max_num,@va_Numero_in,@va_Site_in,@vl_fmc,@vl_cle,@va_Horodate_in,null)
                        end
                end
                else
                begin
                        /*B incremente me dernier numero de franchissement */
                        select  @vl_max_num = @vl_max_num + 1
                        insert EXP..FRN_TDP (numero,zdp,site_zdp,no_fmc,cle_fmc,debut,fin)
                        values (@vl_max_num,@va_Numero_in,@va_Site_in,@vl_fmc,@vl_cle,@va_Horodate_in,null)
                end
        
                /*A     passe au segment suivant */
                fetch fmc into  @vl_fmc,
                                @vl_cle,
                                @vl_type,
                                @vl_classe,
                                @vl_PR
        
                end
                close fmc
                
                
                if ( @vl_max_num = @vl_num_init  )
                        select @vl_reveil = XDC_VRAI
                else
                        select @vl_reveil = XDC_FAUX
                
                declare fmc2 cursor for
                select distinct
                        FMC.numero,
                        FMC.cle,
                        FMC.type,
                        HIS.PR
                from EXP..FMC_GEN FMC,EXP..FMC_HIS HIS
                where (FMC.fin = null or FMC.fin > @va_Horodate_in) and
                 FMC.debut <= @va_Horodate_in  and
                 FMC.type=XZAEC_FMC_PosteOperateur and
                 HIS.horodate_validation in ( select max(horodate_validation)
                                                 from EXP..FMC_HIS
                                                 where numero =  FMC.numero and
                                                        cle =  FMC.cle)                         
                
                open fmc2
        
                fetch fmc2 into @vl_fmc,
                                @vl_cle,
                                @vl_type,
                                @vl_PR
        
        /*A Pour chaque fmc trouvEe : */

                while (@@sqlstatus = 0)
                begin
                
                        if (@vl_cle = @va_Site_in)
                        begin
                                /*B incremente me dernier numero de franchissement */
                                select  @vl_max_num = @vl_max_num + 1
                                insert EXP..FRN_TDP (numero,zdp,site_zdp,no_fmc,cle_fmc,debut,fin)
                                values (@vl_max_num,@va_Numero_in,@va_Site_in,@vl_fmc,@vl_cle,@va_Horodate_in,null)
                                
                                if ( @vl_reveil = XDC_VRAI)
                                begin
                                        select 
                                        @vl_Texte=convert(char(8),getdate(),8)+" DEPASSEMENT SEUIL TDP SUR "+rtrim(ZDP.libelle)+" "+rtrim(AUT.nom)+
                                                "-"+convert(char(7),convert(decimal(7,3),convert(decimal(9,2),GEN.PR)/1000))+
                                                "-"+convert(char(7),convert(decimal(7,3),convert(decimal(9,2),ZDP.PR_fin)/1000))+","+convert(char(1),GEN.sens)
                                        from CFG..EQT_GEN GEN,CFG..EQT_ZDP ZDP, CFG..RES_AUT AUT
                                        where GEN.numero=@va_Numero_in and 
                                              GEN.numero=ZDP.numero and
                                              GEN.type=XDC_EQT_ZDP and
                                              AUT.numero=GEN.autoroute
                                              
                                        if    @vl_type = XZAEC_FMC_PosteOperateur
                                        begin
                        			declare postes cursor for
                        			select  poste_enrichisseur 
                        			from EXP..FMC_GEN 
                        			where   type=XZAEC_FMC_PosteOperateur and
                                			sit=@vl_siteCI and
                                			fin=null
                        
                				open postes
                				fetch postes into @vl_poste
                				while (@@sqlstatus = 0)
                				begin
                        				insert EXP..FMC_REV
                              					( evenement, cle, machine_operateur, texte, sit )
                                        		values ( @vl_fmc, @vl_cle,  @vl_poste, @vl_Texte,  @vl_siteCI)
                        				fetch  postes into @vl_poste
               					 end
                				close postes
                				deallocate cursor postes
                				
                                        end
                                        else
                                        begin
                                        	insert EXP..FMC_REV
                                                	( evenement, cle, machine_operateur, texte, sit )
                                        	select @vl_fmc, @vl_cle,  poste_enrichisseur, @vl_Texte,  @vl_cle
                                        	from EXP..FMC_GEN
                                        	where numero=@vl_fmc and cle=@vl_cle
                                        end
                                end
                        end
        
                /*A     passe au segment suivant */
                fetch fmc2 into @vl_fmc,
                                @vl_cle,
                                @vl_type,
                                @vl_PR
        
                end
                close fmc2
        end
        else
        begin
                update EXP..FRN_TDP set fin=@va_Horodate_in
                where zdp=@va_Numero_in and site_zdp=@va_Site_in and fin=null
        end

        return @vl_Retour
go
