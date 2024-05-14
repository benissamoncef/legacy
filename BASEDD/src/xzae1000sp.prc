/*E*/
/*  Fichier : @(#)xzae1000sp.prc	1.0      Release : 1.0        Date : 19/07/2022
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae1000sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Recherche des FMC, equipements ou fiches annuaire correspondant a un champ de recherche
------------------------------------------------------
* HISTORIQUE :
*
* CGR 	19/07/2022  : creation 1.1
* ABK   26/09/23 	: Ajout district CA (DEM-483)
* GGY	14/11/23	: Ajout PIC (DEM483)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
*
*
* Sequence d'appel
* SP   XZAE1000_Recherche
*
* Arguments en entree
* - champ de recherche
*
* Arguments en sortie
* - liste des FMC, equipements et fiches annuaire correspondant au champ de recherche
*
* Code retour
* XDC_OK
* XDC_NOK
*
* Conditions d'utilisation
*
* Fonction
* Recherche des FMC, equipements ou fiches annuaire correspondant a un champ de recherche
-------------------------------------------------------*/

use PRC
go

drop proc XZAE1000
go

create procedure XZAE1000
	@va_chaine_in		    varchar(25)	= null,
	@va_site_in             T_SITE      = null,
	@va_droitAnnuaire_in    bit         = null,
	@va_droitEquipement_in  bit         = null,
	@va_droitFmc_in         bit         = null
as

    if @va_site_in = (select numero from CFG..RES_DIS where type = XDC_TYPEM_SD)
        begin
        SELECT * FROM (

            /* FICHES ANNUAIRES */
            SELECT 'ANN' AS TYPE_ELEMENT,
                    nom AS LibelleRecherche,
                    convert(varchar,numero) AS ElementSelection1,
                    nom AS ElementSelection2
            from CFG..ANN_FIC
            WHERE @va_droitAnnuaire_in = XDC_OUI
            UNION ALL

            /* EQUIPEMENTS */
            SELECT DISTINCT 'EQT' AS TYPE_ELEMENT,
                    ( CASE
                        WHEN EQT.type = XDC_EQT_PMV THEN 'PMV' || ' ' || convert(varchar, EQT.PR)
                        WHEN EQT.type = XDC_EQT_PMVA THEN 'PMVA' || ' ' || convert(varchar, EQT.PR)
                        WHEN EQT.type = XDC_EQT_BAF THEN 'BAF' || ' ' || convert(varchar, EQT.PR)
                        WHEN EQT.type = XDC_EQT_BAD THEN 'BAD' || ' ' || convert(varchar, EQT.PR)
                        WHEN EQT.type = XDC_EQT_NAV THEN 'BRA' || ' ' || convert(varchar, EQT.PR)
                        WHEN EQT.type = XDC_EQT_PAL THEN 'PAL' || ' ' || convert(varchar, EQT.PR)
                        WHEN EQT.type = XDC_EQT_PRV THEN 'PRV' || ' ' || convert(varchar, EQT.PR)
                        WHEN EQT.type = XDC_EQT_PIC THEN 'PIC' || ' ' || convert(varchar, EQT.PR)
                        WHEN EQT.type = XDC_EQT_CFE THEN 'CFE' || ' ' || convert(varchar, EQT.PR)
                        WHEN EQT.type = XDC_EQT_IMU THEN 'IMU' || ' ' || convert(varchar, EQT.PR)
                        WHEN EQT.type = XDC_EQT_PAU THEN 'PAU' || ' ' || convert(varchar, EQT.PR)
                        ELSE ''
                        END
                    ) AS LibelleRecherche,
                    convert(varchar, EQT.type) AS ElementSelection1,
                    convert(varchar, EQT.numero) AS ElementSelection2
            from CFG..EQT_GEN EQT, EXP..EQT_DSP DSP
            WHERE @va_droitEquipement_in = XDC_OUI
                AND EQT.type IN (XDC_EQT_PMV, XDC_EQT_PMVA, XDC_EQT_BAF, XDC_EQT_BAD, XDC_EQT_NAV, XDC_EQT_PAL, XDC_EQT_PRV, XDC_EQT_PIC, XDC_EQT_CFE, XDC_EQT_IMU, XDC_EQT_PAU)
                AND EQT.eqt_supprime = XDC_NON
                AND EQT.type = DSP.type AND EQT.numero = DSP.equipement
                AND DSP.critique = XDC_NON AND DSP.HS = XDC_NON AND DSP.inhibe = XDC_NON AND DSP.desactive = XDC_NON AND DSP.dernier = XDC_OUI
            UNION ALL

            /* FMC */
            SELECT DISTINCT 'FMC' AS TYPE_ELEMENT,
                    convert(varchar, GEN.numero) || '-' || convert(varchar, GEN.cle) AS LibelleRecherche,
                    convert(varchar,TRT.evenement) AS ElementSelection1,
                    convert(varchar,TRT.cle) AS ElementSelection2
            FROM EXP..FMC_TRT TRT
            left JOIN EXP..FMC_GEN GEN on TRT.cle = GEN.cle and TRT.evenement = GEN.numero
            WHERE @va_droitFmc_in = XDC_OUI

        ) AS ALLTABLE
        WHERE ALLTABLE.LibelleRecherche like '%' + UPPER(@va_chaine_in) + '%'
        end

    else
        begin
        SELECT * FROM (

            /* FICHES ANNUAIRES */
            SELECT 'ANN' AS TYPE_ELEMENT,
                    nom AS LibelleRecherche,
                    convert(varchar,numero) AS ElementSelection1,
                    nom AS ElementSelection2
            from CFG..ANN_FIC
            WHERE @va_droitAnnuaire_in = XDC_OUI
            UNION ALL

            /* EQUIPEMENTS */
            SELECT DISTINCT 'EQT' AS TYPE_ELEMENT,
                    ( CASE
                        WHEN EQT.type = XDC_EQT_PMV THEN 'PMV' || ' ' || convert(varchar, EQT.PR)
                        WHEN EQT.type = XDC_EQT_PMVA THEN 'PMVA' || ' ' || convert(varchar, EQT.PR)
                        WHEN EQT.type = XDC_EQT_BAF THEN 'BAF' || ' ' || convert(varchar, EQT.PR)
                        WHEN EQT.type = XDC_EQT_BAD THEN 'BAD' || ' ' || convert(varchar, EQT.PR)
                        WHEN EQT.type = XDC_EQT_NAV THEN 'BRA' || ' ' || convert(varchar, EQT.PR)
                        WHEN EQT.type = XDC_EQT_PAL THEN 'PAL' || ' ' || convert(varchar, EQT.PR)
                        WHEN EQT.type = XDC_EQT_PRV THEN 'PRV' || ' ' || convert(varchar, EQT.PR)
                        WHEN EQT.type = XDC_EQT_PIC THEN 'PIC' || ' ' || convert(varchar, EQT.PR)
                        WHEN EQT.type = XDC_EQT_CFE THEN 'CFE' || ' ' || convert(varchar, EQT.PR)
                        WHEN EQT.type = XDC_EQT_IMU THEN 'IMU' || ' ' || convert(varchar, EQT.PR)
                        WHEN EQT.type = XDC_EQT_PAU THEN 'PAU' || ' ' || convert(varchar, EQT.PR)
                        ELSE ''
                        END
                    ) AS LibelleRecherche,
                    convert(varchar, EQT.type) AS ElementSelection1,
                    convert(varchar, EQT.numero) AS ElementSelection2
            from CFG..EQT_GEN EQT, EXP..EQT_DSP DSP
            WHERE @va_droitEquipement_in = XDC_OUI
                AND EQT.type IN (XDC_EQT_PMV, XDC_EQT_PMVA, XDC_EQT_BAF, XDC_EQT_BAD, XDC_EQT_NAV, XDC_EQT_PAL, XDC_EQT_PRV, XDC_EQT_PIC, XDC_EQT_CFE, XDC_EQT_IMU, XDC_EQT_PAU)
                AND (EQT.site_gestion = @va_site_in OR EQT.site_gestion = null)
                AND EQT.eqt_supprime = XDC_NON
                AND EQT.type = DSP.type and EQT.numero = DSP.equipement
                AND DSP.critique = XDC_NON AND DSP.HS = XDC_NON AND DSP.inhibe = XDC_NON AND DSP.desactive = XDC_NON AND DSP.dernier = XDC_OUI
            UNION ALL

            /* FMC */
            SELECT DISTINCT 'FMC' AS TYPE_ELEMENT,
                    convert(varchar, GEN.numero) || '-' || convert(varchar, GEN.cle) AS LibelleRecherche,
                    convert(varchar,TRT.evenement) AS ElementSelection1,
                    convert(varchar,TRT.cle) AS ElementSelection2
            FROM EXP..FMC_TRT TRT
            left JOIN EXP..FMC_GEN GEN on TRT.cle = GEN.cle and TRT.evenement = GEN.numero
            WHERE @va_droitFmc_in = XDC_OUI

        ) AS ALLTABLE
        WHERE ALLTABLE.LibelleRecherche like '%' + UPPER(@va_chaine_in) + '%'
        end

	return XDC_OK

go

