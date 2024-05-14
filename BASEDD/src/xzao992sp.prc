/*E*/
/*Fichier :  $Id: xzao992sp.prc,v 1.8 2018/07/11 10:40:00 pc2dpdy Exp $      Release : $Revision: 1.8 $        Date : $Date: 2018/07/11 10:40:00 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao992sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
* AAZ		2007		Premiere creation DEM666 
* JBL		Mai 2008	Reconnaissance des eqt existants par le nom
* JBL		aout 2008	correction FT 1764 : ne fonctionne pas avec les �quipements existants,
*                       Il faut r�initialiser les variables @vl_TypeRecherch, @vl_NumeroRecherche 
*                       avant de les r�utiliser
* JBL       17/11/2008  Correction recuperation numero max d'un eqt pour les capteurs
* LCL	05/07/17		: Modif PRA
* LCL           11/07/18        : correction syntaxe from
------------------------------------------------------*/


/* fichiers inclus */

/* #include "xzao992sp.h" */

/* static char *version = "@(#)xzao992.c    1.1 11/21/07 : xzao992" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajout d'un equipement GTC dans la base
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

drop procedure XZAO992
go

create procedure XZAO992
    @va_Basedd_in           char(3),
    @va_TypeEqtGTC_in       char(100),
    @va_NomEqtGTC_in        char(10),
    @va_Autoroute_in        char(4),
    @va_NomMaitre_in        char(10),
    @va_PR_in               T_PR,
    @va_Sens_in             T_SENS,
    @va_Voie_in             T_VOIE,
    @va_Inverse_in          tinyint,
    @va_Domaine_in          char(2),
    @va_Resultat_out        int output
as 

/* 
*
* SP    XZAO992SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd        va_Basedd_in
* XDY_NomEqt        va_NomEqt_in
* XDY_NomAuto       va_Autoroute_in
* XDY_NomEqt        va_NomMaitre_in
* XDY_PR            va_PR_in
* XDY_Sens          va_Sens_in
* XDY_Voie          va_Voie_in
* XDY_Octet         va_Inverse_in
* XDY_NomEqt        va_Domaine_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf       va_Resultat_out    
*
* VALEUR RENDUE :
*
* Ajoute d'un equipement GTC en base
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
* Insertion ou modification dans la table EQT_GTC_xxx (EQUIPEMENT_DOMAINE xxx domaine)
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* AAZ       21 Nov 2007 : Creation
* GGY		26/09/23 : Ajout district CA (DEM483)
* GGY		17/10/23 : Correction si CFG vide (DEM483)
------------------------------------------------------*/
    declare

    /*A
    ** Definition des variables globales
    */

    @vl_Autoroute      tinyint,
    @vl_Maitre         smallint,
    @vl_TypeEQT        int,
    @vl_NumEQT         smallint,
    @vl_TypeSrv        int,
    @vl_Numero         smallint,
	@vl_TypeMaitre	   int,

	@vl_TypeRecherche	int,
	@vl_NumeroRecherche	smallint,
	
    @vl_GtcRenovee     tinyint,
    @vl_Site			char(10)


    select @va_Resultat_out = XDC_ECHEC

    /*A
    ** Test des parametres d'entree :
    **   Si un des parametres d'entree est egal a null alors XDC_NOK   
    */

    if @va_Basedd_in    = null  
    or @va_NomEqtGTC_in = null 
    or @va_Autoroute_in = null 
    or @va_NomMaitre_in = null
    or @va_PR_in        = null 
    or @va_Sens_in      = null 
    or @va_Voie_in      = null 
    or @va_Inverse_in   = null  
    or @va_Domaine_in   = null  

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

                select @vl_Maitre = numero, @vl_TypeSrv = type, @vl_TypeMaitre = type_maitre
                from   CFG..EQT_GEN
                where  nom   = @va_NomMaitre_in
                and  type  in ( XDC_EQT_TUB,
                                XDC_EQT_ECH,
                                XDC_EQT_LTN,
                                XDC_EQT_HT,
                                XDC_EQT_SYS,
                                XDC_EQT_LI) 



                select @vl_TypeEQT = code
                from   CFG..LIB_PRE
                where  libelle = @va_TypeEqtGTC_in
                  and  notyp   = TYP_EQT 

                /*A
                ** L'autoroute, le serveur et le type equipement existent
                */

                if @vl_Autoroute <> null and @vl_Maitre <> null and @vl_TypeEQT <> null
                begin
                    /* AZAZAZAZA */
                    select @vl_NumEQT = numero
                    from   CFG..EQT_GEN
                    where  type = @vl_TypeEQT
                      and  nom  = @va_NomEqtGTC_in
					  and  maitre = @vl_Maitre
					  and  type_maitre = @vl_TypeSrv
					  
                    if @vl_NumEQT = null
                    begin
						/* ********************************** */
						/* L'equipement est un nouvel equipement --> ajout dans EQT_GEN */
						/* ********************************** */
						/* recherche du numero le plus eleve en tenant compte du type */
						if @vl_TypeEQT in (XDC_EQT_LIT, XDC_EQT_EXT, XDC_EQT_OPA, XDC_EQT_CCO, XDC_EQT_GAB, XDC_EQT_VIT, XDC_EQT_NICHE)
							select @vl_Numero = max(numero)+1 from CFG..EQT_GEN
							                    where type = XDC_EQT_LIT
												  or  type = XDC_EQT_EXT
												  or  type = XDC_EQT_OPA
												  or  type = XDC_EQT_CCO
												  or  type = XDC_EQT_GAB
												  or  type = XDC_EQT_VIT
												  or  type = XDC_EQT_NICHE						
						else
							select @vl_Numero = max(numero) + 1
							from   CFG..EQT_GEN
							where type=@vl_TypeEQT

						/*A
						** Insertion de l'equipement dans la table EQT_GEN
						*/
						if @vl_Numero = null
							select @vl_Numero = 1

						insert into CFG..EQT_GEN ( type,
												   numero,
												   maitre,
												   type_maitre,
												   autoroute,
												   PR,
												   sens,
												   nom)
										  values ( @vl_TypeEQT,
												   @vl_Numero,
												   @vl_Maitre,
												   @vl_TypeSrv,
												   @vl_Autoroute,
												   @va_PR_in,
												   @va_Sens_in,
												   @va_NomEqtGTC_in )

						/* AAZ */

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
                        ** Insertion de l'equipement GTC dans la table EQT_GTC_xxx correspondante
                        */
                        if @va_Domaine_in = 'S'
						begin
                            insert into CFG..EQT_GTC_SIG( type,
                                                          numero,
                                                          type_ouvrage )
                                                 values ( @vl_TypeSrv,
                                                          @vl_Maitre ,
                                                          @vl_TypeMaitre )
                            insert into CFG..EQT_GTC_SIG( type,
                                                          numero,
                                                          type_ouvrage )
                                                 values ( @vl_TypeEQT,
                                                          @vl_Numero ,
                                                          @vl_TypeSrv )
						end
                        if @va_Domaine_in = 'C'
						begin
                            insert into CFG..EQT_GTC_ECL( type,
                                                          numero,
                                                          type_ouvrage )
                                                 values ( @vl_TypeSrv,
                                                          @vl_Maitre ,
                                                          @vl_TypeMaitre )
						  insert into CFG..EQT_GTC_ECL( type,
                                                          numero,
                                                          type_ouvrage )
                                                 values ( @vl_TypeEQT,
                                                          @vl_Numero,
                                                          @vl_TypeSrv )
						end
                        if @va_Domaine_in = 'V'
						begin
                            insert into CFG..EQT_GTC_VEN( type,
                                                          numero,
                                                          type_ouvrage )
                                                 values ( @vl_TypeSrv,
                                                          @vl_Maitre ,
                                                          @vl_TypeMaitre )
							insert into CFG..EQT_GTC_VEN( type,
                                                          numero,
                                                          type_ouvrage )
                                                 values ( @vl_TypeEQT,
                                                          @vl_Numero,
                                                          @vl_TypeSrv )
						end
                        if @va_Domaine_in = 'N'
						begin
                            insert into CFG..EQT_GTC_ENR( type,
                                                          numero,
                                                          type_ouvrage )
                                                 values ( @vl_TypeSrv,
                                                          @vl_Maitre ,
                                                          @vl_TypeMaitre )
						  insert into CFG..EQT_GTC_ENR( type,
                                                          numero,
                                                          type_ouvrage )
                                                 values ( @vl_TypeEQT,
                                                          @vl_Numero,
                                                          @vl_TypeSrv )
						end
                        if @va_Domaine_in = 'I'
						begin
                            insert into CFG..EQT_GTC_INC( type,
                                                          numero,
                                                          type_ouvrage )
                                                 values ( @vl_TypeSrv,
                                                          @vl_Maitre ,
                                                          @vl_TypeMaitre )
						    insert into CFG..EQT_GTC_INC( type, 
                                                          numero,
                                                          type_ouvrage )
                                                 values ( @vl_TypeEQT,
                                                          @vl_Numero,
                                                          @vl_TypeSrv )
						end
                        if @va_Domaine_in = 'X'
						begin
                            insert into CFG..EQT_GTC_EXP( type,
                                                          numero,
                                                          type_ouvrage )
                                                 values ( @vl_TypeSrv,
                                                          @vl_Maitre ,
                                                          @vl_TypeMaitre )
   						    insert into CFG..EQT_GTC_EXP( type,
                                                          numero,
                                                          type_ouvrage )
                                                 values ( @vl_TypeEQT,
                                                          @vl_Numero,
                                                          @vl_TypeSrv )
						end
                        if @va_Domaine_in = 'Y'
						begin
                            insert into CFG..EQT_GTC_SYS( type,
                                                          numero,
                                                          type_ouvrage )
                                                 values ( @vl_TypeSrv,
                                                          @vl_Maitre ,
                                                          @vl_TypeMaitre )
   						    insert into CFG..EQT_GTC_SYS( type,
                                                          numero,
                                                          type_ouvrage )
                                                 values ( @vl_TypeEQT,
                                                          @vl_Numero,
                                                          @vl_TypeSrv )
						end
                        if @va_Domaine_in = 'L'
						begin
                            insert into CFG..EQT_GTC_LTN( type,
                                                          numero,
                                                          type_ouvrage )
                                                 values ( @vl_TypeSrv,
                                                          @vl_Maitre ,
                                                          @vl_TypeMaitre )
 						    insert into CFG..EQT_GTC_LTN( type,
                                                          numero,
                                                          type_ouvrage )
                                                 values ( @vl_TypeEQT,
                                                          @vl_Numero,
                                                          @vl_TypeSrv )
						end
						/* insertion dans EQT_PAN du type Panneau GTC */
						if @vl_TypeEQT = XDC_EQT_PAN and @va_Voie_in <> 0 and @va_Voie_in <> NULL
						begin
							insert into CFG..EQT_PAN( numero, rang_comm, rang_comm2, rang_etat, rang_etat2, bit_disc, bit_disc2, voie, inverse)
												 values ( @vl_Numero,0, 0, 0, 0, 0, 0, @va_Voie_in, @va_Inverse_in)
						end
						/* Lit d'arret */
						if @vl_TypeEQT = XDC_EQT_LIT
						begin
							insert into CFG..EQT_CPT( numero, type, num_bit, num_alerte)
							values ( @vl_Numero,@vl_TypeEQT, 0, 6)
						end	
						/* Extincteur */
						if @vl_TypeEQT = XDC_EQT_EXT
						begin
							insert into CFG..EQT_CPT( numero, type, num_bit, num_alerte)
							values ( @vl_Numero,@vl_TypeEQT, 0, 7)
						end	
						/* Capteur CO */
						if @vl_TypeEQT = XDC_EQT_CCO
						begin
							insert into CFG..EQT_CPT( numero, type, num_bit, num_alerte)
							values ( @vl_Numero,@vl_TypeEQT, 0, 8)
						end	
						/* Capteur Opacite */
						if @vl_TypeEQT = XDC_EQT_OPA
						begin
							insert into CFG..EQT_CPT( numero, type, num_bit, num_alerte)
							values ( @vl_Numero,@vl_TypeEQT, 0, 9)
						end	
						/* Porte des niches */
						if @vl_TypeEQT = XDC_EQT_NICHE
						begin
							insert into CFG..EQT_CPT( numero, type, num_bit, num_alerte)
							values ( @vl_Numero,@vl_TypeEQT, 0, 36)
						end							
                        /* AAZ */ 

                        /*A
                        ** Si erreur d'insertion
                        */

                        if @@rowcount <> 1
                        begin

                            /*A
                            ** Affectation du parametre de sortie Resultat a XDC_AJOUT
                            */

                            select @va_Resultat_out = XDC_AJOUT

                            /*return XDC_NOK*/
                        end   

                        /*A
                        ** Affectation du parametre de sortie Resultat a XDC_AJOUT
                        */

                        select @va_Resultat_out = XDC_AJOUT

                    end

                    else

                    /*A
                    ** L'equipement existe dans EQT_GEN
                    */

                    begin

                        /*A
                        ** Modification de l'quipement dans la table EQT_GEN
                        */
                        update CFG..EQT_GEN
                        set    maitre      = @vl_Maitre,
                               type_maitre = @vl_TypeSrv,
                               autoroute   = @vl_Autoroute,
                               PR          = @va_PR_in,
							   sens		   = @va_Sens_in
                        where  numero = @vl_NumEQT
                          and  type   = @vl_TypeEQT
                          and  nom    = @va_NomEqtGTC_in
						  and  maitre = @vl_Maitre
						  and  type_maitre = @vl_TypeSrv

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

						/* Mise a jour dans EQT_PAN */
						if @vl_TypeEQT = XDC_EQT_PAN and @va_Voie_in <> 0 and @va_Voie_in <> NULL
						begin
							update CFG..EQT_PAN set voie = @va_Voie_in, inverse = @va_Inverse_in where numero = @vl_NumEQT
						end						
						
                        /*A
                        ** Insertion de l'equipement GTC dans la table EQT_GTC_xxx correspondante
                        */
                        if @va_Domaine_in = 'S'
						begin
							/* insertion de l'ouvrage */
							select @vl_TypeRecherche = NULL, @vl_NumeroRecherche = NULL
							select @vl_TypeRecherche = type, @vl_NumeroRecherche = numero
							from CFG..EQT_GTC_SIG
							where type = @vl_TypeSrv and numero = @vl_Maitre and type_ouvrage = @vl_TypeMaitre
							
							if @vl_TypeRecherche IS NULL and @vl_NumeroRecherche IS NULL
	                            insert into CFG..EQT_GTC_SIG( type,
	                                                          numero,
	                                                          type_ouvrage )
	                                                 values ( @vl_TypeSrv,
	                                                          @vl_Maitre ,
	                                                          @vl_TypeMaitre )
							/* insertion de l'equipement */
							select @vl_TypeRecherche = NULL, @vl_NumeroRecherche = NULL
							select @vl_TypeRecherche = type, @vl_NumeroRecherche = numero
							from CFG..EQT_GTC_SIG
							where type = @vl_TypeEQT and numero = @vl_NumEQT and type_ouvrage = @vl_TypeSrv
							if @vl_TypeRecherche IS NULL and @vl_NumeroRecherche IS NULL
	                            insert into CFG..EQT_GTC_SIG( type,
	                                                          numero,
	                                                          type_ouvrage )
	                                                 values ( @vl_TypeEQT,
	                                                          @vl_NumEQT ,
	                                                          @vl_TypeSrv )
						end
                        if @va_Domaine_in = 'C'
						begin
							/* insertion de l'ouvrage */
							select @vl_TypeRecherche = NULL, @vl_NumeroRecherche = NULL
							select @vl_TypeRecherche = type, @vl_NumeroRecherche = numero
							from CFG..EQT_GTC_ECL
							where type = @vl_TypeSrv and numero = @vl_Maitre and type_ouvrage = @vl_TypeMaitre
							if @vl_TypeRecherche IS NULL and @vl_NumeroRecherche IS NULL
	                            insert into CFG..EQT_GTC_ECL( type,
	                                                          numero,
	                                                          type_ouvrage )
	                                                 values ( @vl_TypeSrv,
	                                                          @vl_Maitre ,
	                                                          @vl_TypeMaitre )
							/* insertion de l'equipement */
							select @vl_TypeRecherche = NULL, @vl_NumeroRecherche = NULL
							select @vl_TypeRecherche = type, @vl_NumeroRecherche = numero
							from CFG..EQT_GTC_ECL
							where type = @vl_TypeEQT and numero = @vl_NumEQT and type_ouvrage = @vl_TypeSrv
							if @vl_TypeRecherche IS NULL and @vl_NumeroRecherche IS NULL
								insert into CFG..EQT_GTC_ECL( type,
                                                          numero,
                                                          type_ouvrage )
                                                 values ( @vl_TypeEQT,
                                                          @vl_NumEQT,
                                                          @vl_TypeSrv )
						end
                        if @va_Domaine_in = 'V'
						begin
							/* insertion de l'ouvrage */
							select @vl_TypeRecherche = NULL, @vl_NumeroRecherche = NULL
							select @vl_TypeRecherche = type, @vl_NumeroRecherche = numero
							from CFG..EQT_GTC_VEN
							where type = @vl_TypeSrv and numero = @vl_Maitre and type_ouvrage = @vl_TypeMaitre
							if @vl_TypeRecherche IS NULL and @vl_NumeroRecherche IS NULL
	                            insert into CFG..EQT_GTC_VEN( type,
	                                                          numero,
	                                                          type_ouvrage )
	                                                 values ( @vl_TypeSrv,
	                                                          @vl_Maitre ,
	                                                          @vl_TypeMaitre )
							/* insertion de l'equipement */
							select @vl_TypeRecherche = NULL, @vl_NumeroRecherche = NULL
							select @vl_TypeRecherche = type, @vl_NumeroRecherche = numero
							from CFG..EQT_GTC_VEN
							where type = @vl_TypeEQT and numero = @vl_NumEQT and type_ouvrage = @vl_TypeSrv
							if @vl_TypeRecherche IS NULL and @vl_NumeroRecherche IS NULL
								insert into CFG..EQT_GTC_VEN( type,
	                                                          numero,
	                                                          type_ouvrage )
	                                                 values ( @vl_TypeEQT,
	                                                          @vl_NumEQT,
	                                                          @vl_TypeSrv )
						end
                        if @va_Domaine_in = 'N'
						begin
							/* insertion de l'ouvrage */
							select @vl_TypeRecherche = NULL, @vl_NumeroRecherche = NULL
							select @vl_TypeRecherche = type, @vl_NumeroRecherche = numero
							from CFG..EQT_GTC_ENR
							where type = @vl_TypeSrv and numero = @vl_Maitre and type_ouvrage = @vl_TypeMaitre
							if @vl_TypeRecherche IS NULL and @vl_NumeroRecherche IS NULL
	                            insert into CFG..EQT_GTC_ENR( type,
	                                                          numero,
	                                                          type_ouvrage )
	                                                 values ( @vl_TypeSrv,
	                                                          @vl_Maitre ,
	                                                          @vl_TypeMaitre )
							/* insertion de l'equipement */
							select @vl_TypeRecherche = NULL, @vl_NumeroRecherche = NULL
							select @vl_TypeRecherche = type, @vl_NumeroRecherche = numero
							from CFG..EQT_GTC_ENR
							where type = @vl_TypeEQT and numero = @vl_NumEQT and type_ouvrage = @vl_TypeSrv
							if @vl_TypeRecherche IS NULL and @vl_NumeroRecherche IS NULL
								  insert into CFG..EQT_GTC_ENR( type,
		                                                          numero,
		                                                          type_ouvrage )
		                                                 values ( @vl_TypeEQT,
		                                                          @vl_NumEQT,
		                                                          @vl_TypeSrv )
						end
                        if @va_Domaine_in = 'I'
						begin
							/* insertion de l'ouvrage */
							select @vl_TypeRecherche = NULL, @vl_NumeroRecherche = NULL
							select @vl_TypeRecherche = type, @vl_NumeroRecherche = numero
							from CFG..EQT_GTC_INC
							where type = @vl_TypeSrv and numero = @vl_Maitre and type_ouvrage = @vl_TypeMaitre
							if @vl_TypeRecherche IS NULL and @vl_NumeroRecherche IS NULL
	                            insert into CFG..EQT_GTC_INC( type,
	                                                          numero,
	                                                          type_ouvrage )
	                                                 values ( @vl_TypeSrv,
	                                                          @vl_Maitre ,
	                                                          @vl_TypeMaitre )
							/* insertion de l'equipement */
							select @vl_TypeRecherche = NULL, @vl_NumeroRecherche = NULL
							select @vl_TypeRecherche = type, @vl_NumeroRecherche = numero
							from CFG..EQT_GTC_INC
							where type = @vl_TypeEQT and numero = @vl_NumEQT and type_ouvrage = @vl_TypeSrv
							if @vl_TypeRecherche IS NULL and @vl_NumeroRecherche IS NULL
							    insert into CFG..EQT_GTC_INC( type, 
	                                                          numero,
	                                                          type_ouvrage )
	                                                 values ( @vl_TypeEQT,
	                                                          @vl_NumEQT,
	                                                          @vl_TypeSrv )
						end
                        if @va_Domaine_in = 'X'
						begin
							/* insertion de l'ouvrage */
							select @vl_TypeRecherche = NULL, @vl_NumeroRecherche = NULL
							select @vl_TypeRecherche = type, @vl_NumeroRecherche = numero
							from CFG..EQT_GTC_EXP
							where type = @vl_TypeSrv and numero = @vl_Maitre and type_ouvrage = @vl_TypeMaitre
							if @vl_TypeRecherche IS NULL and @vl_NumeroRecherche IS NULL
	                            insert into CFG..EQT_GTC_EXP( type,
	                                                          numero,
	                                                          type_ouvrage )
	                                                 values ( @vl_TypeSrv,
	                                                          @vl_Maitre ,
	                                                          @vl_TypeMaitre )
							/* insertion de l'equipement */
							select @vl_TypeRecherche = NULL, @vl_NumeroRecherche = NULL
							select @vl_TypeRecherche = type, @vl_NumeroRecherche = numero
							from CFG..EQT_GTC_EXP
							where type = @vl_TypeEQT and numero = @vl_NumEQT and type_ouvrage = @vl_TypeSrv
							if @vl_TypeRecherche IS NULL and @vl_NumeroRecherche IS NULL
	   						    insert into CFG..EQT_GTC_EXP( type,
	                                                          numero,
	                                                          type_ouvrage )
	                                                 values ( @vl_TypeEQT,
	                                                          @vl_NumEQT,
	                                                          @vl_TypeSrv )
						end
                        if @va_Domaine_in = 'Y'
						begin
							/* insertion de l'ouvrage */
							select @vl_TypeRecherche = NULL, @vl_NumeroRecherche = NULL
							select @vl_TypeRecherche = type, @vl_NumeroRecherche = numero
							from CFG..EQT_GTC_SYS
							where type = @vl_TypeSrv and numero = @vl_Maitre and type_ouvrage = @vl_TypeMaitre
							if @vl_TypeRecherche IS NULL and @vl_NumeroRecherche IS NULL
	                            insert into CFG..EQT_GTC_SYS( type,
	                                                          numero,
	                                                          type_ouvrage )
	                                                 values ( @vl_TypeSrv,
	                                                          @vl_Maitre ,
	                                                          @vl_TypeMaitre )
							/* insertion de l'equipement */
							select @vl_TypeRecherche = NULL, @vl_NumeroRecherche = NULL
							select @vl_TypeRecherche = type, @vl_NumeroRecherche = numero
							from CFG..EQT_GTC_SYS
							where type = @vl_TypeEQT and numero = @vl_NumEQT and type_ouvrage = @vl_TypeSrv
							if @vl_TypeRecherche IS NULL and @vl_NumeroRecherche IS NULL
								insert into CFG..EQT_GTC_SYS( type,
															  numero,
															  type_ouvrage )
													 values ( @vl_TypeEQT,
															  @vl_NumEQT,
															  @vl_TypeSrv )
						end
                        if @va_Domaine_in = 'L'
						begin
							/* insertion de l'ouvrage */
							select @vl_TypeRecherche = NULL, @vl_NumeroRecherche = NULL
							select @vl_TypeRecherche = type, @vl_NumeroRecherche = numero
							from CFG..EQT_GTC_LTN
							where type = @vl_TypeSrv and numero = @vl_Maitre and type_ouvrage = @vl_TypeMaitre
							if @vl_TypeRecherche IS NULL and @vl_NumeroRecherche IS NULL
	                            insert into CFG..EQT_GTC_LTN( type,
	                                                          numero,
	                                                          type_ouvrage )
	                                                 values ( @vl_TypeSrv,
	                                                          @vl_Maitre ,
	                                                          @vl_TypeMaitre )
							/* insertion de l'equipement */
							select @vl_TypeRecherche = NULL, @vl_NumeroRecherche = NULL
							select @vl_TypeRecherche = type, @vl_NumeroRecherche = numero
							from CFG..EQT_GTC_LTN
							where type = @vl_TypeEQT and numero = @vl_NumEQT and type_ouvrage = @vl_TypeSrv
							if @vl_TypeRecherche IS NULL and @vl_NumeroRecherche IS NULL
	 						    insert into CFG..EQT_GTC_LTN( type,
	                                                          numero,
	                                                          type_ouvrage )
	                                                 values ( @vl_TypeEQT,
	                                                          @vl_NumEQT,
	                                                          @vl_TypeSrv )
						end						
						
						
                        select @va_Resultat_out = XDC_MODIF

                    end
                end
                else

                    /*A
                    ** Le maitre ou l'autoroute n'existe pas dans EQT_GEN et RES_AUT
                    ** Affectation du parametre de sortie Resultat a XDC_ECHEC
                    */

                    select @va_Resultat_out = XDC_ECHEC

            end

#ifdef CI 

            else

            /*A
            **  Travail dans la base CFT
            */   

            begin

                /*A
                ** Verification de l'existence de l'autoroute et du serveur comme equipement
                */

                select @vl_Autoroute = numero
                from   CFT..RES_AUT
                where  nom = @va_Autoroute_in

                select @vl_Maitre = numero, @vl_TypeSrv = type, @vl_TypeMaitre = type_maitre
                from   CFT..EQT_GEN
                where  nom   = @va_NomMaitre_in
                and  type  in ( XDC_EQT_TUB,
                                XDC_EQT_ECH,
                                XDC_EQT_LTN,
                                XDC_EQT_HT,
                                XDC_EQT_SYS,
                                XDC_EQT_LI) 



                select @vl_TypeEQT = code
                from   CFT..LIB_PRE
                where  libelle = @va_TypeEqtGTC_in
                  and  notyp   = TYP_EQT 

                /*A
                ** L'autoroute, le serveur et le type equipement existent
                */

                if @vl_Autoroute <> null and @vl_Maitre <> null and @vl_TypeEQT <> null
                begin
                    /* AZAZAZAZA */
                    select @vl_NumEQT = numero
                    from   CFT..EQT_GEN
                    where  type = @vl_TypeEQT
                      and  nom  = @va_NomEqtGTC_in
					  and  maitre = @vl_Maitre
					  and  type_maitre = @vl_TypeSrv
					  
                    if @vl_NumEQT = null
                    begin
						/* ********************************** */
						/* L'equipement est un nouvel equipement --> ajout dans EQT_GEN */
						/* ********************************** */
						/* recherche du numero le plus eleve en tenant compte du type */
						if @vl_TypeEQT in (XDC_EQT_LIT, XDC_EQT_EXT, XDC_EQT_OPA, XDC_EQT_CCO, XDC_EQT_GAB, XDC_EQT_VIT, XDC_EQT_NICHE)
							select @vl_Numero = max(numero)+1 from CFT..EQT_GEN
							                    where type = XDC_EQT_LIT
												  or  type = XDC_EQT_EXT
												  or  type = XDC_EQT_OPA
												  or  type = XDC_EQT_CCO
												  or  type = XDC_EQT_GAB
												  or  type = XDC_EQT_VIT
												  or  type = XDC_EQT_NICHE						
						else
							select @vl_Numero = max(numero) + 1
							from   CFT..EQT_GEN
							where type=@vl_TypeEQT

						/*A
						** Insertion de l'equipement dans la table EQT_GEN
						*/
						if @vl_Numero = null
							select @vl_Numero = 1

						insert into CFT..EQT_GEN ( type,
												   numero,
												   maitre,
												   type_maitre,
												   autoroute,
												   PR,
												   sens,
												   nom)
										  values ( @vl_TypeEQT,
												   @vl_Numero,
												   @vl_Maitre,
												   @vl_TypeSrv,
												   @vl_Autoroute,
												   @va_PR_in,
												   @va_Sens_in,
												   @va_NomEqtGTC_in )

						/* AAZ */

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
                        ** Insertion de l'equipement GTC dans la table EQT_GTC_xxx correspondante
                        */
                        if @va_Domaine_in = 'S'
						begin
                            insert into CFT..EQT_GTC_SIG( type,
                                                          numero,
                                                          type_ouvrage )
                                                 values ( @vl_TypeSrv,
                                                          @vl_Maitre ,
                                                          @vl_TypeMaitre )
                            insert into CFT..EQT_GTC_SIG( type,
                                                          numero,
                                                          type_ouvrage )
                                                 values ( @vl_TypeEQT,
                                                          @vl_Numero ,
                                                          @vl_TypeSrv )
						end
                        if @va_Domaine_in = 'C'
						begin
                            insert into CFT..EQT_GTC_ECL( type,
                                                          numero,
                                                          type_ouvrage )
                                                 values ( @vl_TypeSrv,
                                                          @vl_Maitre ,
                                                          @vl_TypeMaitre )
						  insert into CFT..EQT_GTC_ECL( type,
                                                          numero,
                                                          type_ouvrage )
                                                 values ( @vl_TypeEQT,
                                                          @vl_Numero,
                                                          @vl_TypeSrv )
						end
                        if @va_Domaine_in = 'V'
						begin
                            insert into CFT..EQT_GTC_VEN( type,
                                                          numero,
                                                          type_ouvrage )
                                                 values ( @vl_TypeSrv,
                                                          @vl_Maitre ,
                                                          @vl_TypeMaitre )
							insert into CFT..EQT_GTC_VEN( type,
                                                          numero,
                                                          type_ouvrage )
                                                 values ( @vl_TypeEQT,
                                                          @vl_Numero,
                                                          @vl_TypeSrv )
						end
                        if @va_Domaine_in = 'N'
						begin
                            insert into CFT..EQT_GTC_ENR( type,
                                                          numero,
                                                          type_ouvrage )
                                                 values ( @vl_TypeSrv,
                                                          @vl_Maitre ,
                                                          @vl_TypeMaitre )
						  insert into CFT..EQT_GTC_ENR( type,
                                                          numero,
                                                          type_ouvrage )
                                                 values ( @vl_TypeEQT,
                                                          @vl_Numero,
                                                          @vl_TypeSrv )
						end
                        if @va_Domaine_in = 'I'
						begin
                            insert into CFT..EQT_GTC_INC( type,
                                                          numero,
                                                          type_ouvrage )
                                                 values ( @vl_TypeSrv,
                                                          @vl_Maitre ,
                                                          @vl_TypeMaitre )
						    insert into CFT..EQT_GTC_INC( type, 
                                                          numero,
                                                          type_ouvrage )
                                                 values ( @vl_TypeEQT,
                                                          @vl_Numero,
                                                          @vl_TypeSrv )
						end
                        if @va_Domaine_in = 'X'
						begin
                            insert into CFT..EQT_GTC_EXP( type,
                                                          numero,
                                                          type_ouvrage )
                                                 values ( @vl_TypeSrv,
                                                          @vl_Maitre ,
                                                          @vl_TypeMaitre )
   						    insert into CFT..EQT_GTC_EXP( type,
                                                          numero,
                                                          type_ouvrage )
                                                 values ( @vl_TypeEQT,
                                                          @vl_Numero,
                                                          @vl_TypeSrv )
						end
                        if @va_Domaine_in = 'Y'
						begin
                            insert into CFT..EQT_GTC_SYS( type,
                                                          numero,
                                                          type_ouvrage )
                                                 values ( @vl_TypeSrv,
                                                          @vl_Maitre ,
                                                          @vl_TypeMaitre )
   						    insert into CFT..EQT_GTC_SYS( type,
                                                          numero,
                                                          type_ouvrage )
                                                 values ( @vl_TypeEQT,
                                                          @vl_Numero,
                                                          @vl_TypeSrv )
						end
                        if @va_Domaine_in = 'L'
						begin
                            insert into CFT..EQT_GTC_LTN( type,
                                                          numero,
                                                          type_ouvrage )
                                                 values ( @vl_TypeSrv,
                                                          @vl_Maitre ,
                                                          @vl_TypeMaitre )
 						    insert into CFT..EQT_GTC_LTN( type,
                                                          numero,
                                                          type_ouvrage )
                                                 values ( @vl_TypeEQT,
                                                          @vl_Numero,
                                                          @vl_TypeSrv )
						end
						/* insertion dans EQT_PAN du type Panneau GTC */
						if @vl_TypeEQT = XDC_EQT_PAN and @va_Voie_in <> 0 and @va_Voie_in <> NULL
						begin
							insert into CFT..EQT_PAN( numero, rang_comm, rang_comm2, rang_etat, rang_etat2, bit_disc, bit_disc2, voie, inverse)
												 values ( @vl_Numero,0, 0, 0, 0, 0, 0, @va_Voie_in, @va_Inverse_in)
						end
						/* Lit d'arret */
						if @vl_TypeEQT = XDC_EQT_LIT
						begin
							insert into CFT..EQT_CPT( numero, type, num_bit, num_alerte)
							values ( @vl_Numero,@vl_TypeEQT, 0, 6)
						end	
						/* Extincteur */
						if @vl_TypeEQT = XDC_EQT_EXT
						begin
							insert into CFT..EQT_CPT( numero, type, num_bit, num_alerte)
							values ( @vl_Numero,@vl_TypeEQT, 0, 7)
						end	
						/* Capteur CO */
						if @vl_TypeEQT = XDC_EQT_CCO
						begin
							insert into CFT..EQT_CPT( numero, type, num_bit, num_alerte)
							values ( @vl_Numero,@vl_TypeEQT, 0, 8)
						end	
						/* Capteur Opacite */
						if @vl_TypeEQT = XDC_EQT_OPA
						begin
							insert into CFT..EQT_CPT( numero, type, num_bit, num_alerte)
							values ( @vl_Numero,@vl_TypeEQT, 0, 9)
						end	
						/* Porte des niches */
						if @vl_TypeEQT = XDC_EQT_NICHE
						begin
							insert into CFT..EQT_CPT( numero, type, num_bit, num_alerte)
							values ( @vl_Numero,@vl_TypeEQT, 0, 36)
						end							
                        /* AAZ */ 

                        /*A
                        ** Si erreur d'insertion
                        */

                        if @@rowcount <> 1
                        begin

                            /*A
                            ** Affectation du parametre de sortie Resultat a XDC_AJOUT
                            */

                            select @va_Resultat_out = XDC_AJOUT

                            /*return XDC_NOK*/
                        end   

                        /*A
                        ** Affectation du parametre de sortie Resultat a XDC_AJOUT
                        */

                        select @va_Resultat_out = XDC_AJOUT

                    end

                    else

                    /*A
                    ** L'equipement existe dans EQT_GEN
                    */

                    begin

                        /*A
                        ** Modification de l'quipement dans la table EQT_GEN
                        */
                        update CFT..EQT_GEN
                        set    maitre      = @vl_Maitre,
                               type_maitre = @vl_TypeSrv,
                               autoroute   = @vl_Autoroute,
                               PR          = @va_PR_in,
							   sens		   = @va_Sens_in
                        where  numero = @vl_NumEQT
                          and  type   = @vl_TypeEQT
                          and  nom    = @va_NomEqtGTC_in
						  and  maitre = @vl_Maitre
						  and  type_maitre = @vl_TypeSrv

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

						/* Mise a jour dans EQT_PAN */
						if @vl_TypeEQT = XDC_EQT_PAN and @va_Voie_in <> 0 and @va_Voie_in <> NULL
						begin
							update CFT..EQT_PAN set voie = @va_Voie_in, inverse = @va_Inverse_in where numero = @vl_NumEQT
						end						
						
                        /*A
                        ** Insertion de l'equipement GTC dans la table EQT_GTC_xxx correspondante
                        */
                        if @va_Domaine_in = 'S'
						begin
							/* insertion de l'ouvrage */
							select @vl_TypeRecherche = NULL, @vl_NumeroRecherche = NULL
							select @vl_TypeRecherche = type, @vl_NumeroRecherche = numero
							from CFT..EQT_GTC_SIG
							where type = @vl_TypeSrv and numero = @vl_Maitre and type_ouvrage = @vl_TypeMaitre
							if @vl_TypeRecherche IS NULL and @vl_NumeroRecherche IS NULL
	                            insert into CFT..EQT_GTC_SIG( type,
	                                                          numero,
	                                                          type_ouvrage )
	                                                 values ( @vl_TypeSrv,
	                                                          @vl_Maitre ,
	                                                          @vl_TypeMaitre )
							/* insertion de l'equipement */
							select @vl_TypeRecherche = NULL, @vl_NumeroRecherche = NULL
							select @vl_TypeRecherche = type, @vl_NumeroRecherche = numero
							from CFT..EQT_GTC_SIG
							where type = @vl_TypeEQT and numero = @vl_NumEQT and type_ouvrage = @vl_TypeSrv
							if @vl_TypeRecherche IS NULL and @vl_NumeroRecherche IS NULL
	                            insert into CFT..EQT_GTC_SIG( type,
	                                                          numero,
	                                                          type_ouvrage )
	                                                 values ( @vl_TypeEQT,
	                                                          @vl_NumEQT ,
	                                                          @vl_TypeSrv )
						end
                        if @va_Domaine_in = 'C'
						begin
							/* insertion de l'ouvrage */
							select @vl_TypeRecherche = NULL, @vl_NumeroRecherche = NULL
							select @vl_TypeRecherche = type, @vl_NumeroRecherche = numero
							from CFT..EQT_GTC_ECL
							where type = @vl_TypeSrv and numero = @vl_Maitre and type_ouvrage = @vl_TypeMaitre
							if @vl_TypeRecherche IS NULL and @vl_NumeroRecherche IS NULL
	                            insert into CFT..EQT_GTC_ECL( type,
	                                                          numero,
	                                                          type_ouvrage )
	                                                 values ( @vl_TypeSrv,
	                                                          @vl_Maitre ,
	                                                          @vl_TypeMaitre )
							/* insertion de l'equipement */
							select @vl_TypeRecherche = NULL, @vl_NumeroRecherche = NULL
							select @vl_TypeRecherche = type, @vl_NumeroRecherche = numero
							from CFT..EQT_GTC_ECL
							where type = @vl_TypeEQT and numero = @vl_NumEQT and type_ouvrage = @vl_TypeSrv
							if @vl_TypeRecherche IS NULL and @vl_NumeroRecherche IS NULL
								  insert into CFT..EQT_GTC_ECL( type,
		                                                          numero,
		                                                          type_ouvrage )
		                                                 values ( @vl_TypeEQT,
		                                                          @vl_NumEQT,
		                                                          @vl_TypeSrv )
						end
                        if @va_Domaine_in = 'V'
						begin
							/* insertion de l'ouvrage */
							select @vl_TypeRecherche = NULL, @vl_NumeroRecherche = NULL
							select @vl_TypeRecherche = type, @vl_NumeroRecherche = numero
							from CFT..EQT_GTC_VEN
							where type = @vl_TypeSrv and numero = @vl_Maitre and type_ouvrage = @vl_TypeMaitre
							if @vl_TypeRecherche IS NULL and @vl_NumeroRecherche IS NULL
	                            insert into CFT..EQT_GTC_VEN( type,
	                                                          numero,
	                                                          type_ouvrage )
	                                                 values ( @vl_TypeSrv,
	                                                          @vl_Maitre ,
	                                                          @vl_TypeMaitre )
							/* insertion de l'equipement */
							select @vl_TypeRecherche = NULL, @vl_NumeroRecherche = NULL
							select @vl_TypeRecherche = type, @vl_NumeroRecherche = numero
							from CFT..EQT_GTC_VEN
							where type = @vl_TypeEQT and numero = @vl_NumEQT and type_ouvrage = @vl_TypeSrv
							if @vl_TypeRecherche IS NULL and @vl_NumeroRecherche IS NULL
								insert into CFT..EQT_GTC_VEN( type,
	                                                          numero,
	                                                          type_ouvrage )
	                                                 values ( @vl_TypeEQT,
	                                                          @vl_NumEQT,
	                                                          @vl_TypeSrv )
						end
                        if @va_Domaine_in = 'N'
						begin
							/* insertion de l'ouvrage */
							select @vl_TypeRecherche = NULL, @vl_NumeroRecherche = NULL
							select @vl_TypeRecherche = type, @vl_NumeroRecherche = numero
							from CFT..EQT_GTC_ENR
							where type = @vl_TypeSrv and numero = @vl_Maitre and type_ouvrage = @vl_TypeMaitre
							if @vl_TypeRecherche IS NULL and @vl_NumeroRecherche IS NULL
	                            insert into CFT..EQT_GTC_ENR( type,
	                                                          numero,
	                                                          type_ouvrage )
	                                                 values ( @vl_TypeSrv,
	                                                          @vl_Maitre ,
	                                                          @vl_TypeMaitre )
							/* insertion de l'equipement */
							select @vl_TypeRecherche = NULL, @vl_NumeroRecherche = NULL
							select @vl_TypeRecherche = type, @vl_NumeroRecherche = numero
							from CFT..EQT_GTC_ENR
							where type = @vl_TypeEQT and numero = @vl_NumEQT and type_ouvrage = @vl_TypeSrv
							if @vl_TypeRecherche IS NULL and @vl_NumeroRecherche IS NULL
							  insert into CFT..EQT_GTC_ENR( type,
															  numero,
															  type_ouvrage )
													 values ( @vl_TypeEQT,
															  @vl_NumEQT,
															  @vl_TypeSrv )
						end
                        if @va_Domaine_in = 'I'
						begin
							/* insertion de l'ouvrage */
							select @vl_TypeRecherche = NULL, @vl_NumeroRecherche = NULL
							select @vl_TypeRecherche = type, @vl_NumeroRecherche = numero
							from CFT..EQT_GTC_INC
							where type = @vl_TypeSrv and numero = @vl_Maitre and type_ouvrage = @vl_TypeMaitre
							if @vl_TypeRecherche IS NULL and @vl_NumeroRecherche IS NULL
	                            insert into CFT..EQT_GTC_INC( type,
	                                                          numero,
	                                                          type_ouvrage )
	                                                 values ( @vl_TypeSrv,
	                                                          @vl_Maitre ,
	                                                          @vl_TypeMaitre )
							/* insertion de l'equipement */
							select @vl_TypeRecherche = NULL, @vl_NumeroRecherche = NULL
							select @vl_TypeRecherche = type, @vl_NumeroRecherche = numero
							from CFT..EQT_GTC_INC
							where type = @vl_TypeEQT and numero = @vl_NumEQT and type_ouvrage = @vl_TypeSrv
							if @vl_TypeRecherche IS NULL and @vl_NumeroRecherche IS NULL
							    insert into CFT..EQT_GTC_INC( type, 
	                                                          numero,
	                                                          type_ouvrage )
	                                                 values ( @vl_TypeEQT,
	                                                          @vl_NumEQT,
	                                                          @vl_TypeSrv )
						end
                        if @va_Domaine_in = 'X'
						begin
							/* insertion de l'ouvrage */
							select @vl_TypeRecherche = NULL, @vl_NumeroRecherche = NULL
							select @vl_TypeRecherche = type, @vl_NumeroRecherche = numero
							from CFT..EQT_GTC_EXP
							where type = @vl_TypeSrv and numero = @vl_Maitre and type_ouvrage = @vl_TypeMaitre
							if @vl_TypeRecherche IS NULL and @vl_NumeroRecherche IS NULL
	                            insert into CFT..EQT_GTC_EXP( type,
	                                                          numero,
	                                                          type_ouvrage )
	                                                 values ( @vl_TypeSrv,
	                                                          @vl_Maitre ,
	                                                          @vl_TypeMaitre )
							/* insertion de l'equipement */
							select @vl_TypeRecherche = NULL, @vl_NumeroRecherche = NULL
							select @vl_TypeRecherche = type, @vl_NumeroRecherche = numero
							from CFT..EQT_GTC_EXP
							where type = @vl_TypeEQT and numero = @vl_NumEQT and type_ouvrage = @vl_TypeSrv
							if @vl_TypeRecherche IS NULL and @vl_NumeroRecherche IS NULL
	   						    insert into CFT..EQT_GTC_EXP( type,
	                                                          numero,
	                                                          type_ouvrage )
	                                                 values ( @vl_TypeEQT,
	                                                          @vl_NumEQT,
	                                                          @vl_TypeSrv )
						end
                        if @va_Domaine_in = 'Y'
						begin
							/* insertion de l'ouvrage */
							select @vl_TypeRecherche = NULL, @vl_NumeroRecherche = NULL
							select @vl_TypeRecherche = type, @vl_NumeroRecherche = numero
							from CFT..EQT_GTC_SYS
							where type = @vl_TypeSrv and numero = @vl_Maitre and type_ouvrage = @vl_TypeMaitre
							if @vl_TypeRecherche IS NULL and @vl_NumeroRecherche IS NULL
	                            insert into CFT..EQT_GTC_SYS( type,
	                                                          numero,
	                                                          type_ouvrage )
	                                                 values ( @vl_TypeSrv,
	                                                          @vl_Maitre ,
	                                                          @vl_TypeMaitre )
							/* insertion de l'equipement */
							select @vl_TypeRecherche = NULL, @vl_NumeroRecherche = NULL
							select @vl_TypeRecherche = type, @vl_NumeroRecherche = numero
							from CFT..EQT_GTC_SYS
							where type = @vl_TypeEQT and numero = @vl_NumEQT and type_ouvrage = @vl_TypeSrv
							if @vl_TypeRecherche IS NULL and @vl_NumeroRecherche IS NULL
	   						    insert into CFT..EQT_GTC_SYS( type,
	                                                          numero,
	                                                          type_ouvrage )
	                                                 values ( @vl_TypeEQT,
	                                                          @vl_NumEQT,
	                                                          @vl_TypeSrv )
						end
                        if @va_Domaine_in = 'L'
						begin
							/* insertion de l'ouvrage */
							select @vl_TypeRecherche = NULL, @vl_NumeroRecherche = NULL
							select @vl_TypeRecherche = type, @vl_NumeroRecherche = numero
							from CFT..EQT_GTC_LTN
							where type = @vl_TypeSrv and numero = @vl_Maitre and type_ouvrage = @vl_TypeMaitre
							if @vl_TypeRecherche IS NULL and @vl_NumeroRecherche IS NULL
	                            insert into CFT..EQT_GTC_LTN( type,
	                                                          numero,
	                                                          type_ouvrage )
	                                                 values ( @vl_TypeSrv,
	                                                          @vl_Maitre ,
	                                                          @vl_TypeMaitre )
							/* insertion de l'equipement */
							select @vl_TypeRecherche = NULL, @vl_NumeroRecherche = NULL
							select @vl_TypeRecherche = type, @vl_NumeroRecherche = numero
							from CFT..EQT_GTC_LTN
							where type = @vl_TypeEQT and numero = @vl_NumEQT and type_ouvrage = @vl_TypeSrv
							if @vl_TypeRecherche IS NULL and @vl_NumeroRecherche IS NULL
	 						    insert into CFT..EQT_GTC_LTN( type,
	                                                          numero,
	                                                          type_ouvrage )
	                                                 values ( @vl_TypeEQT,
	                                                          @vl_NumEQT,
	                                                          @vl_TypeSrv )
						end						
						
						
                        select @va_Resultat_out = XDC_MODIF

                    end
                end
                else

                    /*A
                    ** Le maitre ou l'autoroute n'existe pas dans EQT_GEN et RES_AUT
                    ** Affectation du parametre de sortie Resultat a XDC_ECHEC
                    */

                    select @va_Resultat_out = XDC_ECHEC

            end

#endif


        end
    end         

    return XDC_OK

go  
