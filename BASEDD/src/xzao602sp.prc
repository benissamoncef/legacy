/*E*/
/*Fichier :  $Id: xzao602sp.prc,v 1.3 2018/07/11 10:48:06 pc2dpdy Exp $      Release : $Revision: 1.3 $        Date : $Date: 2018/07/11 10:48:06 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao602sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao602sp.h" */

/* static char *version = "@(#)xzao602.c    1.1 04/09/07 : xzao602" ; */

/* declaration de fonctions internes */

/* definitions de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajout d'un eqt du  domaine ventilation dans la base
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAO;602
    @va_Basedd_in             char(3),
    @va_NomVEN_in             char(10),
    @va_Autoroute_in          char(4),
    @va_NomServeur_in         char(10),
    @va_PR_in                 T_PR,
    @va_Sens_in               T_SENS,
    @va_Resultat_out        int output
    as 

/* 
*
* SP    XZAO602SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd        va_Basedd_in
* XDY_NomEqt        va_NomVEN_in
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
* Ajout d'un eqt du domaine ventilation en base
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
* Insertion ou modification dans la table EQT_GTC_VEN (EQUIPEMENT VENTILATION)
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* AAZ    04 Sep 2007    : Creation
* LCL	05/07/17		: modif PRA
* LCL	11/07/18        : correction syntaxe from
* GGY	26/09/23		: Ajout district CA (DEM483)
* GGY	17/10/23		: Correction si CFG vide (DEM483)
------------------------------------------------------*/
declare
   
    /*A
    ** Definition des variables globales
    */

    @vl_Autoroute		tinyint,
    @vl_Serveur  		smallint,
    @vl_NumVEN   		smallint,
    @vl_Numero   		smallint,
    @vl_Site			char(10)
   
   
    select @va_Resultat_out = XDC_LU
   
    /*A
    ** Test des parametres d'entree :
    **   Si un des parametres d'entree est egal a null alors XDC_NOK   
    */
      
    if @va_Basedd_in             = null or
       @va_NomVEN_in             = null or 
       @va_Autoroute_in          = null or 
       @va_NomServeur_in         = null or 
       @va_PR_in                 = null or 
       @va_Sens_in               = null
   
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
                where  nom  = @va_NomServeur_in and
                       type = XDC_EQT_NIC

                /*A
                ** L'autoroute et la GTC Nice existent
                */
                     
                if @vl_Autoroute <> null and @vl_Serveur <> null
                begin

                    select @vl_NumVEN = numero
                    from   CFG..EQT_GEN
                    where  type = XDC_EQT_VEN   and
                           nom  = @va_NomVEN_in
                
                    /*A
                    ** L equipement VENTILATION n'existe pas dans EQT_GEN
                    */

                    if @vl_NumVEN = null
                    begin

                        /*A
                        ** Insertion equipement VENTILATION dans la table EQT_GEN
                        */

                        select @vl_Numero = max(numero) 
                        from   CFG..EQT_GEN
                        where  type = XDC_EQT_VEN
              
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
                                          values ( XDC_EQT_VEN,
                                                   @vl_Numero + 1,
                                                   @vl_Serveur,
                                                   XDC_MAI_VEN,
                                                   @vl_Autoroute,
                                                   @va_PR_in,
                                                   @va_Sens_in,
                                                   @va_NomVEN_in )
                  
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
                        ** Insertion equipement VENTILATION dans la table EQT_GTC_VEN
                        */

                        insert into CFG..EQT_GTC_VEN ( type,
                                                       numero,
                                                       type_ouvrage)
                                              values ( XDC_EQT_VEN,
                                                       @vl_Numero + 1, 
                                                       XDC_MAI_VEN)

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
                        ** Affectation du parametre de sortie Resultat a XDC_AJOUT
                        */

                        select @va_Resultat_out = XDC_AJOUT

                    end

                    else

                    /*A
                    ** L equipement existe dans EQT_GEN
                    */

                    begin
               
                        /*A
                        ** Modification de l equipement dans la table EQT_GEN
                        */

                        update CFG..EQT_GEN
                        set    maitre      = @vl_Serveur,
                               type_maitre = XDC_MAI_VEN,
                               autoroute   = @vl_Autoroute,
                               PR          = @va_PR_in
                        where  numero      = @vl_NumVEN  and
                               type        = XDC_EQT_VEN and
                               nom         = @va_NomVEN_in

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
                        ** Modification du eqt VENTILATION dans la table EQT_GTC_VEN
                        */
                        /* A voir */

                        /*

                        update CFG..EQT_GTC_VEN
                        set    adresse_etat        = @va_AdresseEtat_in,
                               adresse_commande    = @va_AdresseCommande_in,
                               adresse_discordance = @va_AdresseDiscordance_in,
                               adresse_telealarme  = @va_AdresseTelealarme_in
                        where  numero = @vl_NumVEN

                        */

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

                    select @vl_Serveur = numero from CFT..EQT_GEN
                    where  nom  = @va_NomServeur_in and
                           type = XDC_EQT_NIC

                    /*A
                    ** L'autoroute et le serveur existent
                    */

                    if @vl_Autoroute <> null and @vl_Serveur <> null
                    begin

	                    select @vl_NumVEN = numero
	                    from   CFT..EQT_GEN
	                    where  type = XDC_EQT_VEN       and
	                           nom  = @va_NomVEN_in

	                    /*A
	                    ** L equipement VENTILATION n'existe pas dans EQT_GEN
	                    */

	                    if @vl_NumVEN = null
	                    begin

	                        /*A
	                        ** Insertion equipement VEN dans la table EQT_GEN
	                        */

	                        select @vl_Numero = max(numero)
	                        from   CFT..EQT_GEN
	                        where  type = XDC_EQT_VEN

	                        if @vl_Numero = null
	                            select @vl_Numero = 0  
            
	                        insert into CFT..EQT_GEN ( type,
	                                                   numero,
	                                                   maitre,
	                                                   type_maitre,
	                                                   autoroute,
	                                                   PR,
	                                                   sens,
	                                                   nom)
	                                          values ( XDC_EQT_VEN,
	                                                   @vl_Numero + 1,
	                                                   @vl_Serveur,
	                                                   XDC_MAI_VEN,
	                                                   @vl_Autoroute,
	                                                   @va_PR_in,
	                                                   @va_Sens_in,
	                                                   @va_NomVEN_in )

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
	                        ** Insertion equipement VENTILATION dans la table EQT_GTC_VEN
	                        */

	                        insert into CFG..EQT_GTC_VEN ( type,
	                                                       numero,
	                                                       type_ouvrage)
	                                              values ( XDC_EQT_VEN,
	                                                       @vl_Numero + 1, 
	                                                       XDC_MAI_VEN)

                  
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
	                    ** Affectation du parametre de sortie Resultat a XDC_AJOUT
	                    */

	                    select @va_Resultat_out = XDC_AJOUT

	                end

	                else

	                /*A
	                ** L'equipement VENTILATION existe dans EQT_GEN
	                */

	                begin

	                    /*A
	                    ** Modification de l'equipement VENTILATION dans la table EQT_GEN
	                    */

	                    update CFT..EQT_GEN
	                    set    maitre      = @vl_Serveur,
	                           type_maitre = XDC_MAI_VEN,
	                           autoroute   = @vl_Autoroute,
	                           PR          = @va_PR_in
	                    where  numero      = @vl_NumVEN and
	                           type        = XDC_EQT_VEN and
	                           nom         = @va_NomVEN_in

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
	                    ** Modification de l'equipement VENTILATION dans la table EQT_GTC_VEN
	                    */

	                    /* A voir */
	                    /*
	                    update CFT..EQT_GTC_VEN
	                    set    adresse_etat        = @va_AdresseEtat_in,
	                           adresse_commande    = @va_AdresseCommande_in,
	                           adresse_discordance = @va_AdresseDiscordance_in,
	                           adresse_telealarme  = @va_AdresseTelealarme_in
	                    where  numero              = @vl_NumVEN
	                    */

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
