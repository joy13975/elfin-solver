#ifndef TEST_CONSTS_H_
#define TEST_CONSTS_H_

#include "geometry.h"

namespace elfin {

/* default test input */
static V3fList const quarter_snake_free_coordinates = {
    Vector3f(82.54196166992188, 3.187546730041504, -44.660125732421875),
    Vector3f(54.139976501464844, -23.924468994140625, -35.15853500366211),
    Vector3f(26.635669708251953, -57.53522872924805, -29.187021255493164),
    Vector3f(21.75318145751953, -63.43537139892578, -1.899409294128418),
    Vector3f(12.520357131958008, -50.98127365112305, 13.686529159545898),
    Vector3f(-4.097459316253662, -37.3050651550293, 18.167621612548828),
    Vector3f(-40.844879150390625, -42.66680908203125, 7.421332359313965)
};

/* kabsch */
static V3fList const points10a = {
    Vector3f(4.7008892286345, 42.938597096873, 14.4318130193692),
    Vector3f(-20.3679194392227, 27.5712678608402, -12.1390617339732),
    Vector3f(24.4692807074156, -1.32083675968276, 31.1580458282477),
    Vector3f(-31.1044984967455, -6.41414114190809, 3.28255887994549),
    Vector3f(18.6775433365315, -5.32162505701938, -14.9272896423117),
    Vector3f(-31.648884426273, -19.3650527983443, 43.9001561999887),
    Vector3f(-13.1515403509663, 0.850865538112699, 37.5942811492984),
    Vector3f(12.561856072969, 1.07715641721097, 5.01563428984222),
    Vector3f(28.0227435151377, 31.7627708322262, 12.2475086001227),
    Vector3f(-41.8874231134215, 29.4831416883453, 8.70447045314168),
};

static V3fList points10b = {
    Vector3f(-29.2257707266972, -18.8897713349587, 9.48960740086143),
    Vector3f(-19.8753669720509, 42.3379642103244, -23.7788252219155),
    Vector3f(-2.90766514824093, -6.9792608670416, 10.2843089382083),
    Vector3f(-26.9511839788441, -31.5183679875864, 21.1215780433683),
    Vector3f(34.4308792695389, 40.4880968679893, -27.825326598276),
    Vector3f(-30.5235710432951, 47.9748378356085, -38.2582349144194),
    Vector3f(-27.4078219027601, -6.11300268738968, -20.3324126781673),
    Vector3f(-32.9291952852141, -38.8880776559401, -18.1221698074118),
    Vector3f(-27.2335702183446, -24.1935304087933, -7.58332402861928),
    Vector3f(-6.43013158961009, -9.12801538874479, 0.785828466111815),
};

static V3fList const points10ab_rot = {
    Vector3f( 0.523673403299203, -0.276948392922051, -0.805646171923458),
    Vector3f(-0.793788382691122, -0.501965361762521, -0.343410511043611),
    Vector3f(-0.309299482996081, 0.819347522879342, -0.482704326238996),
};

static Vector3f const points10ab_tran(-1.08234396236629,
                                      5.08395199432057,
                                      -13.0170407784248);

/* Transform */
static Transform const a_world({
    {   "rot", {
            {0.05532475933432579, 0.017428744584321976, -0.08145802468061447},
            { -0.049986086785793304, 0.08517082780599594, -0.015726475045084953},
            {0.0666375458240509, 0.049418311566114426, 0.055832501500844955}
        }
    },
    {"tran", {8.054840087890625, 1.978692889213562, 3.4368598461151123}}
});

static Transform const a_to_b({
    {   "rot", {
            {0.6502372026443481, 0.3104279935359955, 0.6934162378311157},
            {0.756941020488739, -0.3428647220134735, -0.5563129186630249},
            {0.06505285948514938, 0.8866105675697327, -0.45791906118392944}
        }
    },
    {"tran", { -12.447150230407715, -10.713072776794434, -29.79046058654785}}
});

static Transform const b_world({
    {   "rot", {
            { -0.015099741518497467, 0.08121803402900696, 0.05635272338986397},
            { -0.016968388110399246, -0.058289747685194016, 0.07946307212114334},
            {0.0973862037062645, 0.0024365708231925964, 0.022582994773983955}
        }
    },
    {"tran", {10.415760040283203, 3.510263681411743, 5.347901344299316}}
});

static Transform const c_to_a({
    {   "rot", {
            {0.555465579032898, 0.8273841142654419, -0.08302728086709976},
            { -0.08186252415180206, 0.1537732481956482, 0.9847092628479004},
            {0.8275001645088196, -0.5401752591133118, 0.15314750373363495}
        }
    },
    {"tran", { -11.876138687133789, -42.8249397277832, 10.272198677062988}}
});

static Transform const c_world({
    {   "rot", {
            { -0.03810230642557144, 0.09245651960372925, 9.370781481266022e-05},
            { -0.047751497477293015, -0.01976567879319191, 0.08561023324728012},
            {0.07917074859142303, 0.03257473558187485, 0.0516805462539196}
        }
    },
    {"tran", {5.814658164978027, -1.2366465330123901, 1.102649211883545}}
});

}  /* elfin */

#endif  /* end of include guard: TEST_CONSTS_H_ */