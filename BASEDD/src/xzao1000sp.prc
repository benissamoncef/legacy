/*E*/
/*Fichier :  $Id: xzao1000sp.prc,v 1.4 2018/07/11 11:00:39 pc2dpdy Exp $      Release : $Revision: 1.4 $        Date : $Date: 2018/07/11 11:00:39 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao1000sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------*/
#undef SITE
/* fichiers inclus */

/* #include "xzao1000sp.h" */

/* static char *version = "@(#)xzao1000.c    1.1 11/15/07 : xzao1000" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajout d'une sequence du domaine signalisation dans la base
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAO;1000
    @va_Basedd_in           char(3),
    @va_NomSequence_in      T_NOM,
    @va_Type_in             tinyint,
    @va_Creation_in         bit,
    @va_Resultat_out        int output
    as 

/* 
*
* SP    XZAO1000SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd        va_Basedd_in
* XDY_Nom           va_NomSequence_in
* XDY_Octet         va_Type_in
* XDY_Booleen       va_Creation_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf       va_Resultat_out    
*
* VALEUR RENDUE :
*
* Ajoute une sequence en base
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
* Insertion ou modification dans la table EQT_SEQ_GTC_ENR (EQUIPEMENT_SEQ_ENR)
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* AAZ    15 Nov 2007    : Creation
* LCL           11/07/18        : correction syntaxe from
* GGY	26/09/23 : Ajout district CA (DEM483)
* GGY		17/10/23 : Correction si CFG vide (DEM483)
------------------------------------------------------*/
declare
   
    /*A
    ** Definition des variables globales
    */

    @vl_NumE        smallint,
    @vl_Type        tinyint,
    @vl_NumS        tinyint,
    @vl_Libelle     char(254),
    @vl_Site		char(10)

    select @va_Resultat_out = XDC_ECHEC

    /*A
    ** Test des parametres d'entree :
    **   Si un des parametres d'entree est egal a null alors XDC_NOK   
    */

    if @va_Basedd_in         = null  
    or @va_NomSequence_in    = null 
    or @va_Type_in           = null 
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
                ** Verification de l'existence du type dans LIB_PRE
                */


                /* Controle que le type existe */
                select @vl_Libelle = libelle
                from   CFG..LIB_PRE
                where  code    = @va_Type_in
                  and  notyp   = TYP_EQT


                /*A
                ** Le type ouvrage n'existe pas dans LIB_PRE
                */

                if @vl_Libelle = null
                begin
              
                    /*A
                    ** Le type n'existe pas 
                    ** Affectation du parametre de sortie Resultat a XDC_ECHEC
                    */
           
                    select @va_Resultat_out = XDC_ECHEC
                end
                else
                begin
                    select @vl_NumS = numero  
                    from   CFG..EQT_SEQ_GTC_ENR
                    where  nom        = @va_NomSequence_in
                      and  type       = @va_Type_in

                    /*A
                    ** La sequence n'existe pas dans EQT_SEQ_GTC_ENR
                    */

                    if @vl_NumS = null
                    begin

                        /*A
                        ** Creation positionne =>
                        ** Insertion de la sequence dans la table EQT_SEQ_GTC_ENR
                        */
						select  @vl_NumS = max(numero) + 1
						from   CFG..EQT_SEQ_GTC_ENR
						where  type       = @va_Type_in

						if @vl_NumS = null
							select @vl_NumS = 1

                        if @va_Creation_in = XDC_VRAI
                        begin

                            insert into CFG..EQT_SEQ_GTC_ENR ( numero,
                                                               nom,
                                                               type,
                                                               supprime )
                                                      values ( @vl_NumS,
                                                               @va_NomSequence_in,
                                                               @va_Type_in,
                                                               0)                  

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
						    select @va_Resultat_out = XDC_AJOUT
                        end
						else
						begin
							select @va_Resultat_out = XDC_SUPPR
						end

                    end
                    else
                    begin
					    select @va_Resultat_out = XDC_AJOUT
						
                        if @va_Creation_in <> XDC_VRAI
                        begin
                
                        /*A 
                        ** Creation non positionne =>
                        ** Suppression de la sequence de la table EQT_SEQ_GTC_ENR
                        */
                            delete CFG..EQT_SEQ_GTC_ENR
                            where  type       = @va_Type_in
                              and  numero     = @vl_NumS     

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
							select @va_Resultat_out = XDC_SUPPR
                        end
                    end
                end   
            end
#ifdef CI
			else
			begin
			/* CFT */
				/*A
				** Verification de l'existence du type dans LIB_PRE
				*/
				/* Controle que le type existe */
				select @vl_Libelle = libelle
				from   CFT..LIB_PRE
				where  code    = @va_Type_in
				  and  notyp   = TYP_EQT


				/*A
				** Le type ouvrage n'existe pas dans LIB_PRE
				*/

				if @vl_Libelle = null
				begin
					/*A
					** Le type n'existe pas 
					** Affectation du parametre de sortie Resultat a XDC_ECHEC
					*/
					select @va_Resultat_out = XDC_ECHEC
				end
				else
				begin
					select @vl_NumS = numero  
					from   CFT..EQT_SEQ_GTC_ENR
					where  nom        = @va_NomSequence_in
					  and  type       = @va_Type_in

					/*A
					** La sequence n'existe pas dans EQT_SEQ_GTC_ENR
					*/

					if @vl_NumS = null
					begin

						/*A
						** Creation positionne =>
						** Insertion de la sequence dans la table EQT_SEQ_GTC_ENR
						*/
						select  @vl_NumS = max(numero) + 1
						from   CFT..EQT_SEQ_GTC_ENR
						where  type       = @va_Type_in

						if @vl_NumS = null
							select @vl_NumS = 1

						if @va_Creation_in = XDC_VRAI
						begin

							insert into CFT..EQT_SEQ_GTC_ENR ( numero,
															   nom,
															   type,
															   supprime )
													  values ( @vl_NumS,
															   @va_NomSequence_in,
															   @va_Type_in,
															   0)                  

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
						    select @va_Resultat_out = XDC_AJOUT
						end
						else
						begin
							select @va_Resultat_out = XDC_SUPPR
						end
					end
					else
					begin
					    select @va_Resultat_out = XDC_AJOUT
						if @va_Creation_in <> XDC_VRAI
						begin
				
						/*A 
						** Creation non positionne =>
						** Suppression de la sequence de la table EQT_SEQ_GTC_ENR
						*/
							delete CFT..EQT_SEQ_GTC_ENR
							where  type       = @va_Type_in
							  and  numero     = @vl_NumS     

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
						    select @va_Resultat_out = XDC_SUPPR
						end
					end
				end   
			end	
#endif
        end
    end         

    return XDC_OK

go  
