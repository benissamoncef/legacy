/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef TEWSC_H
#define TEWSC_H

#define tewsc_EnterpriseToEnum_Escota "Escota"
#define tewsc_TypeLocalisation_Autoroute "Autoroute"

#define MOTIF_ESSAI "Essaie"
#define MOTIF_FAUSSE_ALERTE "FausseAlerte"

#define TEWSC_SOURCE_SAE 			"SAE"
#define TEWSC_SOURCE_COMMUNAUTAIRE 	"Communautaire"
#define TEWSC_SOURCE_AUTRE 			"Autre"

#define TEWSC_ETAT_FAUSSEALERTE	"FausseAlerte"
#define TEWSC_ETAT_NONDEFINI	"NonDefini"

#define TEWSC_ACTION_ACQUERIR 0
#define TEWSC_ACTION_OTHER    1

#define TEWSC_SYNTHESE_NULL 0
#define TEWSC_SYNTHESE_OK   1
#define TEWSC_SYNTHESE_KO   2

#define TEWSC_TENDANCE_EGALE      0
#define TEWSC_TENDANCE_POSITIVE   1
#define TEWSC_TENDANCE_NEGATIVE   2
#define TEWSC_TENDANCE_NONDEFINIE 3

typedef struct tewsc_EvenementTO
{
	char Enterprise[20]; /* tewsc_EnterpriseToEnum_Escota */
	char Etat[20];
	char TypeEvenement[50];
	XDY_Horodate HorodateCreation;
	XDY_Horodate HorodateDernierSignalement;
	XDY_Horodate HorodateDerniereModification;
	int Id;
	char Autoroute[10];
	XDY_PR PRDebut;
	XDY_PR PRFin;
	XDY_Sens Sens;
	char TypeLocalisation[20]; /* tewsc_TypeLocalisation_Autoroute */
	int RapprochementComplement_Id;
	int RapprochementId;
	char RapprochementReference[30];
	char RapprochementTypeEvenement[50];
	char Source[20]; /* tewsc_Source_SAE */
	int Synthese;
	int Tendance;
}tewsc_EvenementTO_t;


#endif                          /* TEWSC_H */
