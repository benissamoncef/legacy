/*E*/
/* Fichier : $Id: xzao508sp.prc,v 1.8 2021/10/07 17:45:31 pc2dpdy Exp $        $Revision: 1.8 $        $Date: 2021/10/07 17:45:31 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao508sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Insert IMU en base EQT_GEN
*
------------------------------------------------------
* HISTORIQUE :
*
* ABE 015/08/2022 : Creation NIC et IS DEM-SAE403
------------------------------------------------------ */

/* fichiers inclus */

/* #include "xzao508sp.h" */

/* static char *version = "@(#)xzao508sp.prc	1.1 1.1 07/20/28 : xzao508" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X
------------------------------------------------------
* SERVICE RENDU : 
* Retourne le tunnel
*
* SEQUENCE D'APPEL :
* SP	XZAO508_Rechercher_Nic_IS_proche
*
* Arguments en entree
* XDY_Autoroute	 va_Autoroute_in
* XDY_PR	       va_PR_in
* XDY_Sens	    va_Sens_in
*
* Arguments en sortie
* XDC_char(30) 	va_Libelle_out    
*
------------------------------------------------------
*/

use PRC
go

if exists (select 1 from sysobjects where name = 'XZAO508' and type = 'P')
	drop procedure XZAO508
go

create procedure XZAO508
   @va_Autoroute_in		T_AUTOROUTE,
   @va_PR_in			int,
   @va_Sens_in			tinyint,
   @va_Libelle_out	char(30) = null output
   as 

------------------------------------------------------*/
    /*A
    ** Definition des variables globales
    */
    declare
    @vl_rayon int,
    @vl_element_nom char(10),
    @vl_element_type tinyint,
    @vl_PR1 int, @vl_PR2 int

    /* Mise a vide de l'output */
    select @va_Libelle_out=""

    /*A
    ** Test des parametres d'entree :
    **   Si un des parametres d'entree est egal a null alors XDC_NOK   
    */
        
    if @va_Autoroute_in = null or @va_PR_in = null or
        @va_Sens_in = null
        return XDC_ARG_INV
    else   
    begin

        /* Recherche du rayon dans a table de configuration MGA_PAR_REF */
        select @vl_rayon = CONVERT(INT,valeur)
        from CFG..MGA_PAR_REF 
        where cle = XDC_MGA_PAR_REF_LOC_TUNNEL and code = XDC_MGA_PAR_REF_RAYON_TOUS

        /* Recherche du nom de l'element en fonction du sens */
        if @va_Sens_in = XDC_SENS_SUD
            select top 1 @vl_element_nom = r.nom, @vl_element_type = r.type from CFG..RES_NIC_IS r, CFG..EQT_GEN g, CFG..EQT_TUB t
            where g.numero = t.numero
            and g.type = XDC_EQT_TUB
            and g.autoroute = @va_Autoroute_in
            and g.sens = @va_Sens_in
            and r.autoroute = @va_Autoroute_in
            and r.sens = @va_Sens_in
            and @va_PR_in >= g.PR and @va_PR_in <= t.PR_Influence
            and @va_PR_in >= r.PR - @vl_rayon and @va_PR_in <= r.PR + @vl_rayon 
            order by ABS(r.PR - @va_PR_in) /* Pour prendre la valeur la plus proche */

        else if @va_Sens_in = XDC_SENS_NORD
            select top 1 @vl_element_nom = r.nom, @vl_element_type = r.type from CFG..RES_NIC_IS r, CFG..EQT_GEN g, CFG..EQT_TUB t
            where g.numero = t.numero
            and g.type = XDC_EQT_TUB
            and g.autoroute = @va_Autoroute_in
            and g.sens = @va_Sens_in
            and r.autoroute = @va_Autoroute_in
            and r.sens = @va_Sens_in
            and @va_PR_in >= t.PR_Influence and @va_PR_in <= g.PR
            and @va_PR_in >= r.PR - @vl_rayon and @va_PR_in <= r.PR + @vl_rayon 
            order by ABS(r.PR - @va_PR_in) /* Pour prendre la valeur la plus proche */
        else
            return XDC_ARG_INV
        
        /* Si on a un element dans le rayon du PR_in */
        if @@rowcount != 0
            if @vl_element_type = XDC_RES_NIC_IS_TYPE_NICHE
                select @va_Libelle_out = XZAEC_SYN_NICHE + @vl_element_nom

            else if @vl_element_type = XDC_RES_NIC_IS_TYPE_ISSUE_DE_SECOUR
                select @va_Libelle_out = XZAEC_SYN_ISSUE_DE_SECOUR + @vl_element_nom

            else
                return XDC_NOK
        else
            return XDC_NOK
    end
        return XDC_OK         
go  
