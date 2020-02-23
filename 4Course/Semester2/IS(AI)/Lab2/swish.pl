band(twentyOnePilots).
band(edSheeran).
band(fooFighters).

solo(edSheeran).
solo(shawnMendes).

musicianRole(tylerJoseph, singer).
musicianRole(tylerJoseph, piano).
musicianRole(tylerJoseph, bassGuitar).
musicianRole(joshuaDun, drums).
musicianRole(daveGrouhl, singer).
musicianRole(daveGrouhl, guitar).
musicianRole(ramiJaffe, piano).
musicianRole(taylorHawkins, drums).
musicianRole(nateMendel, bassGuitar).
musicianRole(pateSmear, guitar).
musicianRole(edSheeran, singer).
musicianRole(edSheeran, guitar).
musicianRole(daveGrouhl, drums).
musicianRole(kurtCobain, singer).
musicianRole(kurtCobain, guitar).
musicianRole(kristNovoselic, bassGuitar).
musicianRole(shawnMendes, singer).
musicianRole(shawnMendes, guitar).

musicianBand(tylerJoseph, twentyOnePilots).
musicianBand(joshuaDun, twentyOnePilots).
musicianBand(kurtCobain, nirvana).
musicianBand(daveGrouhl, nirvana).
musicianBand(daveGrouhl, fooFighters).
musicianBand(ramiJaffe, fooFighters).
musicianBand(taylorHawkins, fooFighters).
musicianBand(nateMendel, fooFighters).
musicianBand(pateSmear, fooFighters).
musicianBand(edSheeran, edSheeran).
musicianBand(shawnMendes, shawnMendes).

rolesInBandsForArtist(X, Y):-
    musicianRole(X, Y).

listOfBandsForArtist(X, Y) :-
    musicianBand(X, Y).

whoPlaysInstrument(X, Y):-
    musicianRole(X, Y).

isSoloArtist(X):-
     solo(X).

isBand(X):-
    band(X).

whoPlaysInBand(X, Z):-
    musicianBand(X, Z).