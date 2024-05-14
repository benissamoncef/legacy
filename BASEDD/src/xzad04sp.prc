/*E*/
/*  Fichier : $Id: xzad04sp.prc,v 1.4 2018/05/03 13:45:22 devgfi Exp $      Release : $Revision: 1.4 $        Date : $Date: 2018/05/03 13:45:22 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzad04sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* fournit le liste des franchissement pour un site , 
* une horodate et une fmc donnes.
*
------------------------------------------------------
* HISTORIQUE :
*
* P.N.	24/02/97	: Creation	(dem/tdp) (V 1.1)
* P.N.	24/10/97	: corrections	(dem/tdp) (V 1.2)
* P.N.  17/09/1998	: separation exp/his dem/1696 + msg tfm au CI dem/1713 1.3
* JPL	03/05/18	: Suppression de la procedure avant compilation  1.4
* ABK	26/09/23 : Ajout district CA (DEM-483)
-----------------------------------------------------*/


/*X*/
/*-----------------------------------------------------
* Service rendu
* fournit le liste des franchissement pour un site , 
* une horodate et une fmc donnEs.
* 
* Sequence d'appel
* SP    XZAD04_Liste_Franchissement
* 
* Arguments en entree
* XDY_Octet             va_NumeroSite_in        : site 
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
* Consultation de la table TDP_FRN
-------------------------------------------------------*/

use PRC
go

if exists (select 1 from sysobjects where name = 'XZAD04' and type = 'P')
	drop procedure XZAD04
go


create procedure XZAD04
        @va_NumeroSite_in               tinyint         = null,
        @va_Horodate_in         datetime        = null,
        @va_NoFMC_in            int             = null,
        @va_CleFMC_in           tinyint         = null
        
as
        declare  @vl_Retour int,
   		@vl_Status int


#ifdef HIST
	exec @vl_Status = SQL_CI.PRC..XZAD04
        @va_NumeroSite_in,
        @va_Horodate_in,
        @va_NoFMC_in,
        @va_CleFMC_in

        if @vl_Status = null
                return XDC_PRC_INC
        else return @vl_Status
#else

        select @vl_Retour = XDC_OK

        /*A controle parametres en entree */
        if @va_Horodate_in = null or
           @va_Horodate_in = XDC_DATE_NULLE or
           @va_NoFMC_in = null or 
           @va_CleFMC_in = null 
                return XDC_ARG_INV

        if exists (select * from EXP..SAD_PLN
                         where evenement = @va_NoFMC_in and
                         cle = @va_CleFMC_in)
        begin

        /*A recherche le dernier franchissement de seuil pour la FMC qui 
                s'est produits apres le dernier calcul de plan d'action de la FMC*/
        if ( @va_NumeroSite_in = null ) or (@va_NumeroSite_in in (select numero from CFG..RES_DIS
                                            where type = XDC_TYPEM_SD))
        begin
                select distinct
                        zdp,
                        fin
                
                from    EXP..FRN_TDP FRN, EXP..SAD_PLN PLN
                where   
                         (PLN.evenement = @va_NoFMC_in and
                         PLN.cle = @va_CleFMC_in and
                         PLN.horodate <= @va_Horodate_in and
                         PLN.horodate in (select max(horodate)
                                      from EXP..SAD_PLN
                                      where evenement = @va_NoFMC_in  and
                                            cle = @va_CleFMC_in )
                         ) and 
                         (fin >= PLN.horodate or fin = null ) and
                        no_fmc = @va_NoFMC_in and
                        cle_fmc = @va_CleFMC_in
                group by zdp
                having debut=max(debut)
        end
        else
        begin
                select distinct
                        zdp,
                        fin
                
                from    EXP..FRN_TDP FRN, EXP..SAD_PLN PLN
                where   site_zdp = @va_NumeroSite_in and
                        no_fmc = @va_NoFMC_in and
                        cle_fmc = @va_CleFMC_in and
                        (
                         (PLN.evenement = @va_NoFMC_in and
                         PLN.cle = @va_CleFMC_in and
                         PLN.horodate <= @va_Horodate_in and
                         PLN.horodate in ( select max(horodate)
                                           from EXP..SAD_PLN
                                           where evenement = @va_NoFMC_in  and
                                                 cle = @va_CleFMC_in )
                         ) and
                         (fin >= PLN.horodate or fin = null))
                group by zdp
                having debut=max(debut)
        end
        end

        else
        begin
        if ( @va_NumeroSite_in = null ) or (@va_NumeroSite_in in (select numero from CFG..RES_DIS
                                            where type = XDC_TYPEM_SD))
        begin
                select distinct
                        zdp,
                        fin
                
                from    EXP..FRN_TDP
                where   
                        fin = null and
                        no_fmc = @va_NoFMC_in and
                        cle_fmc = @va_CleFMC_in
                group by zdp
                having debut=max(debut)
        end
        else
        begin
                select distinct
                        zdp,
                        fin
                
                from    EXP..FRN_TDP 
                where   site_zdp = @va_NumeroSite_in and
                        no_fmc = @va_NoFMC_in and
                        cle_fmc = @va_CleFMC_in and
                        fin = null
                group by zdp
                having debut=max(debut)
        end


        end
        return @vl_Retour
#endif
go






