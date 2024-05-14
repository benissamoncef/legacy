/*E*/
/*Fichier :  $Id: xzao982sp.prc,v 1.4 2018/07/11 11:08:08 pc2dpdy Exp $      Release : $Revision: 1.4 $        Date : $Date: 2018/07/11 11:08:08 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao982sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------*/
/* fichiers inclus */

/* #include "xzao982sp.h" */

/* static char *version = "@(#)xzao982.c    1.1 11/15/07 : xzao982" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajout d'une sequence autorisee ddu domaine signalisation dans la base
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAO;982
    @va_Basedd_in           char(3),
    @va_NomSequence_in      T_NOM,
    @va_Type_in             tinyint,
    @va_NomSIG_in           T_NOM,
    @va_Creation_in         bit,
    @va_Resultat_out        int output
    as 

/* 
*
* SP    XZAO982SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd        va_Basedd_in
* XDY_Nom           va_NomSequence_in
* XDY_Octet         va_Type_in
* XDY_Nom           va_NomSIG_in
* XDY_Booleen       va_Creation_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf       va_Resultat_out    
*
* VALEUR RENDUE :
*
* Ajoute une sequence autorisee d'un tunnel en base
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
* Insertion ou modification dans la table EQT_SQA_GTC_SIG (EQUIPEMENT_SQA_SIG)
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* AAZ    15 Nov 2007    : Creation
* LCL	05/07/17	: Modif PRA
* LCL           11/07/18        : correction syntaxe from
* PNI   16/06/2023 : Ajout du type d'ouvrage dans la recherche de la seq SAE-492
* GGY	26/09/23 : Ajout district CA (DEM483)
* GGY	17/10/23 : Correction si CFG vide (DEM483)
------------------------------------------------------*/
declare
   
    /*A
    ** Definition des variables globales
    */

    @vl_NumSIG      smallint,
    @vl_NumE        smallint,
    @vl_Type        tinyint,
    @vl_NumSeq      tinyint,
    @vl_NumS        tinyint,
    @vl_Site		char(10)

    select @va_Resultat_out = XDC_ECHEC

    /*A
    ** Test des parametres d'entree :
    **   Si un des parametres d'entree est egal a null alors XDC_NOK   
    */

    if @va_Basedd_in         = null  
    or @va_NomSequence_in    = null 
    or @va_Type_in           = null 
    or @va_NomSIG_in         = null
    or @va_Creation_in       = null 

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
			             ** Verification de l'existence de la sequence dans EQT_SEQ_GTC_SIG
				*/
                select @vl_NumSeq = numero
                from   CFG..EQT_SEQ_GTC_SIG
                where  nom = @va_NomSequence_in 
                      and  type = @va_Type_in

                /*A
                ** La sequence existe dans EQT_SEQ_GTC_SIG
                */

                if @vl_NumSeq <> null
                begin

                    select @vl_NumSIG = numero
                    from   CFG..EQT_GEN
                    where  nom  = @va_NomSIG_in
                      and  type = @va_Type_in
                    /*A
                    ** L equipement du domaine SIG existe dans EQT_GEN
                    */

                    if @vl_NumSIG <> null
                    begin
               
                        select @vl_NumE   = equipement, @vl_NumS = sequence  
                        from   CFG..EQT_SQA_GTC_SIG
                        where  type       = @va_Type_in
                          and  equipement = @vl_NumSIG
                          and  sequence   = @vl_NumSeq

                        /*A
                        ** La sequence autorisee du tunnel n'existe pas dans EQT_SQA_GTC_SIG
                        */

                        if @vl_NumE = null and @vl_NumS = null
                        begin
               
                            /*A
                            ** Creation positionne =>
                            ** Insertion de la sequence autorisee d'un tunnel dans la table EQT_SQA_TUB
                            */

                            if @va_Creation_in = XDC_VRAI
                            begin

                                insert into CFG..EQT_SQA_GTC_SIG ( type,
                                                                   equipement,
                                                                   sequence )
                                                          values ( @va_Type_in,
                                                                   @vl_NumSIG,
                                                                   @vl_NumSeq )                  

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
                                 ** Creation non positionne =>
                                 ** Affectation du parametre de sortie Resultat a XDC_SUPPR
                                 */
               
                                select @va_Resultat_out = XDC_SUPPR
                  
                          end

                          else
                 
                          /*A
                          ** La sequence autorisee n'existe pas dans EQT_SQA_GTC_SIG
                          */
                  
                          begin
							 select @va_Resultat_out = XDC_AJOUT
                  
                             if @va_Creation_in <> XDC_VRAI
                    
                             /*A 
                             ** Creation non positionne =>
                             ** Suppression de la sequence autorisee dand la table EQT_SQA_GTC_SIG
                             */
             
                            begin
               
                                delete CFG..EQT_SQA_GTC_SIG
                                where  type       = @va_Type_in
                                  and  equipement = @vl_NumSIG
                                  and  sequence   = @vl_NumSeq     

                                /*A
                                ** Si erreur de suppression
                                */

                                if @@rowcount <> 1
                                begin

                                   /*A
                                   ** Affectation du parametre de sortie Resultat a XDC_SUPPR
                                   */

                                   select @va_Resultat_out = XDC_SUPPR

                                   return XDC_NOK
                                end

                                /*A
                                ** Affectation du parametre de sortie Resultat a XDC_SUPPR
                                */

                                select @va_Resultat_out = XDC_SUPPR
                            end
                        end
                    end   
                  
                    else
                  
                        /*A
                        ** L'equipement n'existe pas 
                        ** Affectation du parametre de sortie Resultat a XDC_ECHEC
                        */
               
                        select @va_Resultat_out = XDC_ECHEC
                end

                else
               
                    /*A
                    ** La sequence du tunnel n'existe pas 
                    ** Affectation du parametre de sortie Resultat a XDC_AJOUT
                    */
               
                    select @va_Resultat_out = XDC_AJOUT
                    
            end

#ifdef CI

            else
         
            /*A
            **  Travail dans la base CFT
            */   

            begin


                /*A
                ** Verification de l'existence de la sequence dans EQT_SEQ_GTC_SIG
                */

                select @vl_NumSeq = numero
                from   CFT..EQT_SEQ_GTC_SIG
                where  nom = @va_NomSequence_in
                      and  type = @va_Type_in

                /*A
                ** La sequence existe dans EQT_SEQ_GTC_SIG
                */

                if @vl_NumSeq <> null
                begin

                    select @vl_NumSIG = numero
                    from   CFT..EQT_GEN
                    where  nom  = @va_NomSIG_in
                      and  type = @va_Type_in
                    /*A
                    ** L equipement du domaine SIG existe dans EQT_GEN
                    */

                    if @vl_NumSIG <> null
                    begin
               
                        select @vl_NumE   = equipement, @vl_NumS = sequence  
                        from   CFT..EQT_SQA_GTC_SIG
                        where  type       = @va_Type_in
                          and  equipement = @vl_NumSIG
                          and  sequence   = @vl_NumSeq

                        /*A
                        ** La sequence autorisee du tunnel n'existe pas dans EQT_SQA_GTC_SIG
                        */

                        if @vl_NumE = null and @vl_NumS = null
                        begin
               
                            /*A
                            ** Creation positionne =>
                            ** Insertion de la sequence autorisee d'un tunnel dans la table EQT_SQA_TUB
                            */

                            if @va_Creation_in = XDC_VRAI
                            begin

                                insert into CFT..EQT_SQA_GTC_SIG ( type,
                                                                   equipement,
                                                                   sequence )
                                                          values ( @va_Type_in,
                                                                   @vl_NumSIG,
                                                                   @vl_NumSeq )                  

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
                                 ** Creation non positionne =>
                                 ** Affectation du parametre de sortie Resultat a XDC_SUPPR
                                 */
               
                                select @va_Resultat_out = XDC_SUPPR
                  
                          end

                          else
                 
                          /*A
                          ** La sequence autorisee n'existe pas dans EQT_SQA_GTC_SIG
                          */
                  
                          begin
							 select @va_Resultat_out = XDC_AJOUT
                  
                             if @va_Creation_in <> XDC_VRAI
                    
                             /*A 
                             ** Creation non positionne =>
                             ** Suppression de la sequence autorisee dand la table EQT_SQA_GTC_SIG
                             */
             
                            begin
               
                                delete CFT..EQT_SQA_GTC_SIG
                                where  type       = @va_Type_in
                                  and  equipement = @vl_NumSIG
                                  and  sequence   = @vl_NumSeq     

                                /*A
                                ** Si erreur de suppression
                                */

                                if @@rowcount <> 1
                                begin

                                   /*A
                                   ** Affectation du parametre de sortie Resultat a XDC_SUPPR
                                   */

                                   select @va_Resultat_out = XDC_SUPPR

                                   return XDC_NOK
                                end

                                /*A
                                ** Affectation du parametre de sortie Resultat a XDC_SUPPR
                                */

                                select @va_Resultat_out = XDC_SUPPR
                            end
                        end
                    end   
                  
                    else
                  
                        /*A
                        ** L'equipement n'existe pas 
                        ** Affectation du parametre de sortie Resultat a XDC_ECHEC
                        */
               
                        select @va_Resultat_out = XDC_ECHEC
                end

                else
               
                    /*A
                    ** La sequence du tunnel n'existe pas 
                    ** Affectation du parametre de sortie Resultat a XDC_ECHEC
                    */

                    select @va_Resultat_out = XDC_AJOUT

            end 
#endif


        end
    end         

    return XDC_OK

go  
