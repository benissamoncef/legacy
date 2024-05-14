/*E*/
/*Fichier :  @(#)xzao980sp.prc	1.2      Release : 1.2        Date : 07/09/08
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao980sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao980sp.h" */

/* static char *version = "@(#)xzao980.c    1.1 20/09/94 : xzao980" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajout d'un demi echangeur dans la base
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAO;980
    @va_Basedd_in                char(3),
    @va_NomDemiEch_in            char(10),
    @va_Autoroute_in             char(4),
    @va_NomServeur_in            char(10),
    @va_PR_in                    T_PR,
    @va_Sens_in                  T_SENS,
    @va_Resultat_out             int output
    as 

/* 
*
* SP    XZAO980SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd        va_Basedd_in
* XDY_NomEqt        va_NomDemiEch_in
* XDY_NomAuto       va_Autoroute_in
* XDY_NomEqt        va_NomServeur_in
* XDY_PR            va_PR_in
* XDY_Sens          va_Sens_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf       va_Resultat_out    
*
* VALEUR RENDUE :
*
* Ajoute d'un demi echangeur en base
* 
* CODE RETOUR :
*
* 0    XDC_OK
* 1    XDC_NOK 
*
* CONDITIONS D'UTILISATION :
*
* Config OFF LINE
*
* FONCTION :
*
* Insertion ou modification dans la table EQT_ECH (EQUIPEMENT_ECHANGEUR)
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* AAZ       15 Nov 2007    : Creation
* LCL		05/07/17	: Modif PRA
* LCL           11/07/18        : correction syntaxe from
* GGY	26/09/23 : Ajout district CA (DEM483)
* GGY		17/10/23 : Correction si CFG vide (DEM483)
------------------------------------------------------*/
declare
   
    /*A
    ** Definition des variables globales
    */

    @vl_Autoroute   tinyint,
    @vl_Serveur     smallint,
    @vl_NumEch      smallint,
    @vl_Numero      smallint,
    @vl_Site		char(10)

    select @va_Resultat_out = XDC_LU

    /*A
    ** Test des parametres d'entree :
    **   Si un des parametres d'entree est egal a null alors XDC_NOK   
    */

    if @va_Basedd_in     = null 
    or @va_NomDemiEch_in = null 
    or @va_Autoroute_in  = null 
    or @va_NomServeur_in = null
    or @va_PR_in         = null 
    or @va_Sens_in       = null 

        return XDC_ARG_INV

    else   
    begin

		/* Verification que le site est bien celui du CI ou CA */
		select  @vl_Site = @@servername
	   
		/* Si le site est CI ou CA */
		if @vl_Site=XDC_SQLCI or @vl_Site=XDC_SQLCA
		begin

            /*A
            **  Travail dans la base CFG
            */   

            if @va_Basedd_in = XDC_BASE_CFG
            begin

                /*A
                ** Verification de l'existence de l'autoroute et du serveur comme equipement
                */

                select @vl_Autoroute = numero
                from   CFG..RES_AUT
                where  nom = @va_Autoroute_in

                select @vl_Serveur = numero
                from   CFG..EQT_GEN
                where  nom  = @va_NomServeur_in
                  and  type = XDC_EQT_NIC

                /*A
                ** L'autoroute et la GTC Nice existent
                */

                if @vl_Autoroute <> null and @vl_Serveur <> null
                begin

                    select @vl_NumEch = numero 
                    from   CFG..EQT_GEN
                    where  type        = XDC_EQT_ECH
                      and  type_maitre = XDC_EQT_NIC
                      and  nom         = @va_NomDemiEch_in

                    /*A
                    ** Le demi-echangeur n'existe pas dans EQT_GEN
                    */

                    if @vl_NumEch = null
                    begin

                        /*A
                        ** Insertion du demi echangeur dans la table EQT_GEN
                        */

                        select @vl_Numero = max(numero)
                        from   CFG..EQT_GEN
                        where  type = XDC_EQT_ECH

                        if @vl_Numero = null
                            select @vl_Numero = 0  

                        insert into CFG..EQT_GEN ( type,
                                                   numero,
                                                   maitre,
                                                   type_maitre,
                                                   autoroute,
                                                   PR,
                                                   sens,
                                                   nom)
                                          values ( XDC_EQT_ECH,
                                                   @vl_Numero + 1,
                                                   @vl_Serveur,
                                                   XDC_EQT_NIC,
                                                   @vl_Autoroute,
                                                   @va_PR_in,
                                                   @va_Sens_in,
                                                   @va_NomDemiEch_in )

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
                        ** Insertion du demi echangeur dans la table EQT_ECH
                        ** Pas utilise dans la GTC renovee
                        */

                        /*A
                        ** Affectation du parametre de sortie Resultat a XDC_AJOUT
                        */
         
                        select @va_Resultat_out = XDC_AJOUT

                    end

                    else

                    /*A
                    ** Le demi-echangeur existe dans EQT_GEN
                    */

                    begin
               
                        /*A
                        ** Modification du demi echangeur dans la table EQT_GEN
                        */

                        update CFG..EQT_GEN
                        set    maitre      = @vl_Serveur,
                               type_maitre = XDC_EQT_NIC,
                               autoroute   = @vl_Autoroute,
                               PR          = @va_PR_in
                        where  numero = @vl_NumEch
                          and  type   = XDC_EQT_ECH
                          and  nom    = @va_NomDemiEch_in

                        /*A
                        ** Si erreur de modification
                        */

                        if @@rowcount <> 1
                        begin

                            /*A
                            ** Affectation du parametre de sortie Resultat a XDC_MODIF
                            */

                            select @va_Resultat_out = XDC_MODIF

                            return XDC_NOK
                        end   

                        /*A
                        ** Modification du demi echangeur dans la table EQT_ECH
                        ** Pas utilise dand la GTC Renovee
                        */

                        /*A
                        ** Affectation du parametre de sortie Resultat a XDC_MODIF
                        */

                        select @va_Resultat_out = XDC_MODIF

                    end
                end

                else

                    /*A
                    ** La GTC Nice ou l'autoroute n'existe pas dans EQT_GEN et RES_AUT
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

                /*A
                ** Verification de l'existence de l'autoroute et du serveur comme equipement
                */

                select @vl_Autoroute = numero
                from   CFT..RES_AUT
                where  nom = @va_Autoroute_in

                select @vl_Serveur = numero
                from   CFT..EQT_GEN
                where  nom  = @va_NomServeur_in
                  and  type = XDC_EQT_NIC

                /*A
                ** L'autoroute et le serveur existent
                */

                if @vl_Autoroute <> null and @vl_Serveur <> null
                begin

                    select @vl_NumEch = numero
                    from   CFT..EQT_GEN
                    where  type       = XDC_EQT_ECH
                      and type_maitre = XDC_EQT_NIC
                      and  nom        = @va_NomDemiEch_in

                    /*A
                    ** Le demi-echangeur n'existe pas dans EQT_GEN
                    */

                    if @vl_NumEch = null
                    begin

                        /*A
                        ** Insertion du demi echangeur dans la table EQT_GEN
                        */

                        select @vl_Numero = max(numero)
                        from   CFT..EQT_GEN
                        where  type = XDC_EQT_ECH

                        if @vl_Numero = null
                            select @vl_Numero = 0  

                        insert into CFT..EQT_GEN ( type,
                                                   numero,
                                                   maitre,
                                                   type_maitre,
                                                   autoroute,
                                                   PR,
                                                   sens,
                                                   nom )
                                          values ( XDC_EQT_ECH, 
                                                   @vl_Numero + 1,
                                                   @vl_Serveur,
                                                   XDC_EQT_NIC,
                                                   @vl_Autoroute,
                                                   @va_PR_in,
                                                   @va_Sens_in,
                                                   @va_NomDemiEch_in )

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
                        ** Insertion du demi echangeur dans la table EQT_ECH
                        ** Pas utilise dans la GTC Renovee
                        */
                  
                        /*A
                        ** Affectation du parametre de sortie Resultat a XDC_AJOUT
                        */

                        select @va_Resultat_out = XDC_AJOUT

                    end

                    else

                    /*A
                    ** Le demi-echangeur existe dans EQT_GEN
                    */

                    begin

                        /*A
                        ** Modification du demi echangeur dans la table EQT_GEN
                        */

                        update CFT..EQT_GEN
                        set maitre      = @vl_Serveur,
                            type_maitre = XDC_EQT_NIC,
                            autoroute   = @vl_Autoroute,
                            PR          = @va_PR_in
                        where numero = @vl_NumEch
                          and type   = XDC_EQT_ECH
                          and nom    = @va_NomDemiEch_in

                        /*A
                        ** Si erreur de modification
                        */

                        if @@rowcount <> 1
                        begin

                            /*A
                            ** Affectation du parametre de sortie Resultat a XDC_MODIF
                            */

                            select @va_Resultat_out = XDC_MODIF

                            return XDC_NOK
                        end   

                        /*A
                        ** Modification du demi echangeur dans la table EQT_ECH
                        ** Pas utilise dans la GTC renovee
                        */

                        /*A
                        ** Affectation du parametre de sortie Resultat a XDC_MODIF
                        */

                        select @va_Resultat_out = XDC_MODIF

                    end
                end

                else

                    /*A
                    ** Le serveur ou l'autoroute n'existe pas dans EQT_GEN et RES_AUT
                    ** Affectation du parametre de sortie Resultat a XDC_ECHEC
                    */

                    select @va_Resultat_out = XDC_ECHEC

            end

#endif

        end
    end         

    return XDC_OK

go  
