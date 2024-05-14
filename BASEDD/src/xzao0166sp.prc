/*E*/
/*Fichier :  $Id: xzao0166sp.prc,v 1.4 2018/07/11 10:26:02 pc2dpdy Exp $      Release : $Revision: 1.4 $        Date : $Date: 2018/07/11 10:26:02 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao0166sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao0166sp.h" */

/* static char *version = "@(#)xzao0166.c    1.1 11/12/2007 : xzao0166" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajout d'une GTC NICE RENOVEE dans la base
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAO;1166
   @va_Basedd_in        	char(3),
   @va_Nom_in			char(10),
   @va_NomMachine_in		char(10),
   @va_LongueurITOR_in		smallint,
   @va_AdresseITOR_in		smallint,
   @va_LongueurETOR_in		smallint,
   @va_AdresseETOR_in		smallint,
   @va_LongTelealarme_in	smallint,
   @va_AdresseTelealarme_in	smallint,
   @va_LongTelecomEch_in	smallint,
   @va_AdresseTelecomEch_in	smallint,
   @va_LongTelecomTub_in	smallint,
   @va_AdresseTelecomTub_in	smallint,
   @va_LongTelesignEch_in	smallint,
   @va_AdresseTelesignEch_in	smallint,
   @va_LongTelesignTub_in	smallint,
   @va_AdresseTelesignTub_in	smallint,
   @va_LongSeqEch_in		smallint,
   @va_AdresseSeqEch_in		smallint,
   @va_LongSeqTub_in		smallint,
   @va_AdresseSeqTub_in		smallint,
   @va_LongAlerteTub_in		smallint,
   @va_AdresseAlerteTub_in	smallint,
   @va_Resultat_out		int output
   as 

/* 
*
* SP    XZAO166SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd		va_Basedd_in
* XDY_NomEqt		va_Nom_in
* XDY_NomMachine	va_NomMachine_in
* XDY_Mot		va_LongueurITOR_in
* XDY_Mot		va_AdresseITOR_in
* XDY_Mot		va_LongueurETOR_in
* XDY_Mot		va_AdresseETOR_in
* XDY_Mot		va_LongTelealarme_in
* XDY_Mot		va_AdresseTelealarme_in
* XDY_Mot		va_LongTelecomEch_in
* XDY_Mot		va_AdresseTelecomEch_in
* XDY_Mot		va_LongTelecomTub_in
* XDY_Mot		va_AdresseTelecomTub_in
* XDY_Mot		va_LongTelesignEch_in
* XDY_Mot		va_AdresseTelesignEch_in
* XDY_Mot		va_LongTelesignTub_in
* XDY_Mot		va_AdresseTelesignTub_in
* XDY_Mot		va_LongSeqEch_in
* XDY_Mot		va_AdresseSeqEch_in
* XDY_Mot		va_LongSeqTub_in
* XDY_Mot		va_AdresseSeqTub_in
* XDY_Mot		va_LongAlerteTub_in
* XDY_Mot		va_AdresseAlerteTub_in
*
*
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf       va_Resultat_out    
*
* VALEUR RENDUE :
*
* Ajoute une GTC NICE RENOVEE en base
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
* Insertion ou modification dans la table EQT_NIC (GTC_NICE)
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic    12 Nov 2007	: Creation
* LCL		04/07/17	: Modif PRA
* LCL		11/07/18	: correction syntaxe from
* GGY		26/09/23	: Ajout district CA (DEM483)
* GGY		17/10/23	: Correction si CFG vide (DEM483)
------------------------------------------------------*/
declare
   
    /*A
    ** Definition des variables globales
    */
    
    @vl_Machine		smallint,
    @vl_NumGTC		smallint,
    @vl_Numero		smallint,
    @vl_Site		char(10)
    
    select @va_Resultat_out = XDC_LU
        
    /*A
    ** Test des parametres d'entree :
    **   Si un des parametres d'entree est egal a null alors XDC_NOK   
    */
    
    if @va_Basedd_in            = null  
    or @va_Nom_in               = null
    or @va_NomMachine_in        = null 
    
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
                ** Verification de l'existence et recherche de la machine comme equipement
                */
         
                select @vl_Machine = numero 
                from   CFG..EQT_GEN
                where  nom  = @va_NomMachine_in
                  and  type = XDC_EQT_MAC

                /*A
                ** Existence de la machine
                */
            
                if @vl_Machine <> null
                begin
               
                    select @vl_NumGTC = numero
                    from   CFG..EQT_GEN
                    where  type   = XDC_EQT_NIC
                      and  nom    = @va_Nom_in 
                  
                   
                    /*A
                    ** La GTC NICE RENOVEE n'existe pas comme equipement
                    */
                    
                    if @vl_NumGTC = null
                    begin
                  
                        /*A
                        ** Insertion de la GTC RENOVEE dans la table EQT_GEN
                        */
               
                        select @vl_Numero = numero
                        from   CFG..EQT_GEN
                        where  type = XDC_EQT_NIC
                    
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
                                          values ( XDC_EQT_NIC,
                                                   @vl_Numero + 1,
                                                   @vl_Machine,
                                                   XDC_MAI_NIC,
                                                   null,
                                                   null,
                                                   null,
                                                   @va_Nom_in )
                  
                        /*A
                        ** Erreur d'insertion
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
                        ** Insertion de la GTC RENOVEE dans la table EQT_NIC
                        */
               
                        insert into CFG..EQT_NIC ( numero,
                                                   longueur_ITOR,
                                                   adresse_ITOR,
                                                   longueur_ETOR,
                                                   adresse_ETOR,
                                                   longueur_telealarme,
                                                   adresse_telealarme,
                                                   longueur_telecom_ech,
                                                   adresse_telecom_ech,
                                                   longueur_telecom_tub,
                                                   adresse_telecom_tub,
                                                   longueur_telesign_ech,
                                                   adresse_telesign_ech,
                                                   longueur_telesign_tub,
                                                   adresse_telesign_tub,
                                                   longueur_sequence_ech,
                                                   adresse_sequence_ech,
                                                   longueur_sequence_tub,
                                                   adresse_sequence_tub,
                                                   longueur_alerte_tub,
                                                   adresse_alerte_tub )
                                          values ( @vl_Numero + 1,
                                                   @va_LongueurITOR_in,
                                                   @va_AdresseITOR_in,
                                                   @va_LongueurETOR_in,
                                                   @va_AdresseETOR_in,
                                                   @va_LongTelealarme_in,
                                                   @va_AdresseTelealarme_in,
                                                   @va_LongTelecomEch_in,
                                                   @va_AdresseTelecomEch_in,
                                                   @va_LongTelecomTub_in,
                                                   @va_AdresseTelecomTub_in,
                                                   @va_LongTelesignEch_in,
                                                   @va_AdresseTelesignEch_in,
                                                   @va_LongTelesignTub_in,
                                                   @va_AdresseTelesignTub_in,
                                                   @va_LongSeqEch_in,
                                                   @va_AdresseSeqEch_in,
                                                   @va_LongSeqTub_in,
                                                   @va_AdresseSeqTub_in,
                                                   @va_LongAlerteTub_in,
                                                   @va_AdresseAlerteTub_in )
                  
                        /*A
                        ** Erreur d'insertion
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
                    ** La GTC RENOVEE existe dans EQT_GEN
                    */

                    begin
                     
                        /*A
                        ** Modification de la GTC RENOVEE Nice
                        */
                    
                        update CFG..EQT_GEN
                        set    maitre      = @vl_Machine,
                               type_maitre = XDC_MAI_NIC
                        where  numero      = @vl_NumGTC
                          and  type        = XDC_EQT_NIC
                          and  nom         = @va_Nom_in 

                        /*A
                        ** Erreur de modification
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
                        ** Modification de la GTC RENOVEE Nice
                        */

                        update CFG..EQT_NIC
                        set    longueur_ITOR         = @va_LongueurITOR_in,
                               adresse_ITOR          = @va_AdresseITOR_in,
                               longueur_ETOR         = @va_LongueurETOR_in,
                               adresse_ETOR          = @va_AdresseETOR_in,
                               longueur_telealarme   = @va_LongTelealarme_in,
                               adresse_telealarme    = @va_AdresseTelealarme_in,
                               longueur_telecom_ech  = @va_LongTelecomEch_in,
                               adresse_telecom_ech   = @va_AdresseTelecomEch_in,
                               longueur_telecom_tub  = @va_LongTelecomTub_in,
                               adresse_telecom_tub   = @va_AdresseTelecomTub_in,
                               longueur_telesign_ech = @va_LongTelesignEch_in,
                               adresse_telesign_ech  = @va_AdresseTelesignEch_in,
                               longueur_telesign_tub = @va_LongTelesignTub_in,
                               adresse_telesign_tub  = @va_AdresseTelesignTub_in,
                               longueur_sequence_ech = @va_LongSeqEch_in,
                               adresse_sequence_ech  = @va_AdresseSeqEch_in,
                               longueur_sequence_tub = @va_LongSeqTub_in,
                               adresse_sequence_tub  = @va_AdresseSeqTub_in,
                               longueur_alerte_tub   = @va_LongAlerteTub_in,
                               adresse_alerte_tub    = @va_AdresseAlerteTub_in
                      
                        where numero = @vl_NumGTC
                         
                        /*A
                        ** Erreur de modification
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
                    ** La machine n'existe pas comme equipement
                    **
                    ** Affectation du parametre de sortie Resultat a XDC_ECHEC
                    */
         
                    select @va_Resultat_out = XDC_ECHEC
            end
      
#ifdef CI
            /*A
            **  Travail dans la base CFT
            */   
      
            begin
         
                /*A
                ** Verification de l'existence et recherche de la machine comme equipement
                */
         
                select @vl_Machine = numero
                from   CFT..EQT_GEN
                where  type = XDC_EQT_MAC
                  and  nom  = @va_NomMachine_in

                /*A
                ** Existence de la machine
                */
            
                if @vl_Machine <> null
                begin
               
                    select @vl_NumGTC = numero
                    from   CFT..EQT_GEN
                    where  type = XDC_EQT_NIC
                      and  nom  = @va_Nom_in 
                   
                    /*A
                    ** La GTC NICE RENOVEE n'existe pas comme equipement
                    */

                    if @vl_NumGTC = null
                    begin

                        /*A
                        ** Insertion de la GTC RENOVEE dans la table EQT_GEN
                        */

                        select @vl_Numero = numero
                        from   CFT..EQT_GEN
                        where  type = XDC_EQT_NIC

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
                                          values ( XDC_EQT_NIC,
                                                   @vl_Numero + 1,
                                                   @vl_Machine,
                                                   XDC_MAI_NIC,
                                                   null,
                                                   null,
                                                   null,
                                                   @va_Nom_in )
                  
                        /*A
                        ** Erreur d'insertion
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
                        ** Insertion de la GTC RENOVEE dans la table EQT_NIC
                        */

                        insert into CFT..EQT_NIC ( numero,
                                                   longueur_ITOR,
                                                   adresse_ITOR,
                                                   longueur_ETOR,
                                                   adresse_ETOR,
                                                   longueur_telealarme,
                                                   adresse_telealarme,
                                                   longueur_telecom_ech,
                                                   adresse_telecom_ech,
                                                   longueur_telecom_tub,
                                                   adresse_telecom_tub,
                                                   longueur_telesign_ech,
                                                   adresse_telesign_ech,
                                                   longueur_telesign_tub,
                                                   adresse_telesign_tub,
                                                   longueur_sequence_ech,
                                                   adresse_sequence_ech,
                                                   longueur_sequence_tub,
                                                   adresse_sequence_tub,
                                                   longueur_alerte_tub,
                                                   adresse_alerte_tub )
                                          values ( @vl_Numero + 1,
                                                   @va_LongueurITOR_in,
                                                   @va_AdresseITOR_in,
                                                   @va_LongueurETOR_in,
                                                   @va_AdresseETOR_in,
                                                   @va_LongTelealarme_in,
                                                   @va_AdresseTelealarme_in,
                                                   @va_LongTelecomEch_in,
                                                   @va_AdresseTelecomEch_in,
                                                   @va_LongTelecomTub_in,
                                                   @va_AdresseTelecomTub_in,
                                                   @va_LongTelesignEch_in,
                                                   @va_AdresseTelesignEch_in,
                                                   @va_LongTelesignTub_in,
                                                   @va_AdresseTelesignTub_in,
                                                   @va_LongSeqEch_in,
                                                   @va_AdresseSeqEch_in,
                                                   @va_LongSeqTub_in,
                                                   @va_AdresseSeqTub_in,
                                                   @va_LongAlerteTub_in,
                                                   @va_AdresseAlerteTub_in )
              
                        /*A
                        ** Erreur d'insertion
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
                    ** La GTC RENOVEE existe dans EQT_GEN
                    */

                    begin

                        /*A
                        ** Modification de la GTC Nice RENOVEE
                        */

                        update CFT..EQT_GEN
                        set    maitre      = @vl_Machine,
                               type_maitre = XDC_MAI_NIC
                        where  numero      = @vl_NumGTC
                          and  type        = XDC_EQT_NIC
                          and  nom         = @va_Nom_in 

                        /*A
                        ** Erreur de modification
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
                        ** Modification de la GTC Nice RENOVEE
                        */

                        update CFT..EQT_NIC
                        set    longueur_ITOR         = @va_LongueurITOR_in,
                               adresse_ITOR          = @va_AdresseITOR_in,
                               longueur_ETOR         = @va_LongueurETOR_in,
                               adresse_ETOR          = @va_AdresseETOR_in,
                               longueur_telealarme   = @va_LongTelealarme_in,
                               adresse_telealarme    = @va_AdresseTelealarme_in,
                               longueur_telecom_ech  = @va_LongTelecomEch_in,
                               adresse_telecom_ech   = @va_AdresseTelecomEch_in,
                               longueur_telecom_tub  = @va_LongTelecomTub_in,
                               adresse_telecom_tub   = @va_AdresseTelecomTub_in,
                               longueur_telesign_ech = @va_LongTelesignEch_in,
                               adresse_telesign_ech  = @va_AdresseTelesignEch_in,
                               longueur_telesign_tub = @va_LongTelesignTub_in,
                               adresse_telesign_tub  = @va_AdresseTelesignTub_in,
                               longueur_sequence_ech = @va_LongSeqEch_in,
                               adresse_sequence_ech  = @va_AdresseSeqEch_in,
                               longueur_sequence_tub = @va_LongSeqTub_in,
                               adresse_sequence_tub  = @va_AdresseSeqTub_in,
                               longueur_alerte_tub   = @va_LongAlerteTub_in,
                               adresse_alerte_tub    = @va_AdresseAlerteTub_in
                      
                        where numero = @vl_NumGTC

                        /*A
                        ** Erreur de modification
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
                ** La machine n'existe pas comme equipement
                **
                ** Affectation du parametre de sortie Resultat a XDC_ECHEC
                */

                select @va_Resultat_out = XDC_ECHEC
            end
#endif

        end
    end         
     
    return XDC_OK
         
go  
