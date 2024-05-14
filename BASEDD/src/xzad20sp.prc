/*E*/
/*  Fichier : $Id: xzad20sp.prc,v 1.1 2019/12/12 17:29:58 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2019/12/12 17:29:58 $
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
* JMG	10/12/19 	creation DEM1365 1.0
* ABK	26/09/23 : Ajout district CA (DEM-483)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* 
* 
* Sequence d'appel
* SP    XZAD20
* 
* Arguments en entree
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

drop procedure XZAD20
go


create procedure XZAD20

        @va_Numero_in           int        = null, 
        @va_Cle_in             tinyint         = null
        
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
                @vl_site_voisin tinyint,
                @vl_siteCI tinyint

                
                
        select @vl_Retour = XDC_OK

        /* initialisation variable locale */
        select @vl_siteCI = numero 
                from CFG..RES_DIS
                where type = XDC_TYPEM_SD

                declare fmc2 cursor for
                select distinct
                        FMC.numero,
                        FMC.cle,
                        FMC.type,
			poste_enrichisseur
                from EXP..FMC_GEN FMC
                where FMC.fin = null  and
                 FMC.debut <= getdate()  and
		 FMC.cle= vl_siteCI and 
                 FMC.type=XZAEC_FMC_PosteOperateur              
                
                open fmc2
        
                fetch fmc2 into @vl_fmc,
                                @vl_cle,
                                @vl_type,
				@vl_poste
        
        /*A Pour chaque fmc trouvEe : */

                while (@@sqlstatus = 0)
                begin
					select @vl_fmc
                                        select 
                                        @vl_Texte="SURCHARGE OU INHIBITION TDP A VERIFIER"
					insert EXP..FMC_REV
                              					( evenement, cle, machine_operateur, texte, sit )
                                        		values ( @vl_fmc, @vl_cle,  @vl_poste, @vl_Texte,  @vl_cle)
        		
                	/*A     passe au segment suivant */
                	fetch fmc2 into @vl_fmc,
                                	@vl_cle,
                                	@vl_type,
					@vl_poste
        
                end
                close fmc2
        return @vl_Retour
go
