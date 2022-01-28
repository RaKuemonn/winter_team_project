#include "stage_spawner.h"

#include "deco_egg.h"
#include "deco_furniture.h"
#include "stage.h"

#include "stage_1.h"
#include "stage_1_movement.h"
#include "stage_1_obj.h"

#include "stage_2.h"
#include "stage_2_movement.h"
#include "stage_2_obj.h"

#include "stage_3.h"
#include "stage_3_movement.h"
#include "stage_3_move.h"
#include "stage_3_goal.h"

#include "stage_4.h"
#include "stage_4_movement.h"
#include "stage_4_move.h"
#include "stage_4_goal.h"

#include "stage_boss.h"

#include "deco_leaf.h"


void Stage_Spawner::set_stage_1()
{
    set_stage<Stage_1>({ 0.0f,-10.0f,0.0f });
    set_stage<Stage_1_Movement>({ 0.0f,-3.0f,50.0f });
    set_stage<Stage_1_Obj>({ 0.0f,-10.0f,0.0f });

    set_stage<Deco_Wood_Spring>( { -25.1f,-9.0f,-88.0f } );
    set_stage<Deco_Wood_Spring>( { -14.8f,-9.0f,-79.0f } );
    set_stage<Deco_Wood_Spring>( { -18.5f,-9.0f,-66.7f } );
    set_stage<Deco_Wood_Spring>( { -12.5f,-9.0f,-59.3f } );
    set_stage<Deco_Wood_Spring>( { -21.5f,-9.0f,-49.7f } );
    set_stage<Deco_Wood_Spring>( { -11.5f,-9.0f,-45.3f } );
    set_stage<Deco_Wood_Spring>( { 11.5f ,-9.0f,-42.3f } );
    set_stage<Deco_Wood_Spring>( { 17.1f ,-9.0f,-46.6f } );
    set_stage<Deco_Wood_Spring>( { 24.7f ,-9.0f,-51.1f } );
    set_stage<Deco_Wood_Spring>( { 17.6f ,-9.0f,-56.0f } );
    set_stage<Deco_Wood_Spring>( { 24.0f ,-9.0f,-62.0f } );
    set_stage<Deco_Wood_Spring>( { 17.0f ,-9.0f,-67.0f } );
    set_stage<Deco_Wood_Spring>( { 24.9f ,-9.0f,-71.6f } );
    set_stage<Deco_Wood_Spring>( { 25.6f ,-9.0f,-77.8f } );
    set_stage<Deco_Wood_Spring>( { 19.5f ,-9.0f,-75.9f } );
    set_stage<Deco_Wood_Spring>( { 18.6f ,-9.0f,-83.9f } );
    set_stage<Deco_Wood_Spring>( { 25.7f ,-9.0f,-86.6f } );
    set_stage<Deco_Wood_Spring>({ -10.5f, 0.1f , 80.5f} );
    set_stage<Deco_Wood_Spring>({ 16.1f ,-3.3f,  97.6f} );
    set_stage<Deco_Wood_Spring>({ 11.7f ,-3.3f,110.1f } );
    set_stage<Deco_Wood_Spring>({ -7.0f  ,-3.3f,113.2f} );
    set_stage<Deco_Wood_Spring>({ -14.3f,-3.3f,96.0f  } );

    set_stage<Deco_Egg>({11.803970, -10.033720, -99.282333}  );
    set_stage<Deco_Egg2>({9.339858, -10.033720, -96.554993}   );
    set_stage<Deco_Egg>({-11.508363, -10.036642, -118.157463});
    set_stage<Deco_Egg>({-14.992005, -10.036642, -113.102028});
    set_stage<Deco_Egg>({5.707535, -2.990594, -17.761417}    );
    set_stage<Deco_Egg>({-7.017325, -3.361440, 119.214546}   );
    set_stage<Deco_Egg2>({7.806339, -3.361440, 119.611053}    );
    set_stage<Deco_Egg>({9.836212, -3.361440, 119.312317}    );
    set_stage<Deco_Egg>({-9.059424, 0.169580, 80.945442}     );
}

void Stage_Spawner::set_stage_2()
{
    set_stage<Stage_2>({ 0.0f,0.0f,0.0f });
    set_stage<Stage_2_Movement>({ 0.0f,10.0f,90.0f });
    set_stage<Stage_2_Obj>({ 0.0f,0.0f,0.0f });

    set_stage<Deco_Wood_Summer>({ 7.877774,7.570627,-11.401569 });
    set_stage<Deco_Wood_Summer>({ -6.882060,13.866771,-0.278979 });
    set_stage<Deco_Wood_Summer>({ 8.464912,7.797373,36.292599 });
    set_stage<Deco_Wood_Summer>({ 8.090108, 7.730286, 45.232483 });
    set_stage<Deco_Wood_Summer>({ -8.174158, 7.806430, 35.904316 });
    set_stage<Deco_Wood_Summer>({-9.180177, 7.502372, -7.400256});
    set_stage<Deco_Wood_Summer>({7.598135, 7.502372, -6.595610} );
    set_stage<Deco_Wood_Summer>({ -4.157318, 13.881007, 50.737782 });

    set_stage<Deco_Tetrapos>({-8.573287, 7.418993, -15.270934});
    set_stage<Deco_Umbrella_and_Bed>({8.181915, 13.784978, 1.058876}  );
    set_stage<Deco_Ball>({7.911190, 13.863326, 50.424065} );
}

void Stage_Spawner::set_stage_3()
{
    set_stage<Stage_3>({ 0.0f,-10.0f,0.0f });
    set_stage<Stage_3_Movement>({ 0.0f, -8.0f, -7.0f });
    set_stage<Stage_3_Move>({ 0.0f, -8.0f,-76.0f });
    set_stage<Stage_3_Move>({ 0.0f, -4.0f, 75.0f });
    set_stage<Stage_3_Move>({ 0.0f, -2.0f, 109.0f })->set_start({ -15.0f,-2.0f,109.0f }).set_end({ 15.0f,-2.0f,109.0f });
    set_stage<Stage_3_Goal>({ -29.0f, 0.0f, 154.0f });

    set_stage<Deco_Wood_Fall>({ -10.993289, -9.631988, -54.259415 });
    set_stage<Deco_Wood_Fall>({ -8.553539, -9.631988, -47.005100 });
    set_stage<Deco_Wood_Fall>({ -9.532264, -9.631988, -40.415230 });
    set_stage<Deco_Wood_Fall>({ -8.191775, -9.631988, -26.191259 });
    set_stage<Deco_Wood_Fall>({ 12.157725, -9.631988, -33.208740 });
    set_stage<Deco_Wood_Fall>({ 10.280873, -9.631988, -43.045330 });
    set_stage<Deco_Wood_Fall>({ 12.313251, -9.636354, 10.463448 });
    set_stage<Deco_Wood_Fall>({ 9.501500, -9.636354, 19.285145 });
    set_stage<Deco_Wood_Fall>({ -17.522652, -9.636354, 40.123573 });
    set_stage<Deco_Wood_Fall>({ -11.460207, -9.636354, 61.839260 });
    set_stage<Deco_Wood_Fall>({ 11.810927, -9.636354, 57.535389 });
    set_stage<Deco_Wood_Fall>({ -15.240294, -9.635165, 53.474854 });
    set_stage<Deco_Wood_Fall>({ 18.217279, -9.632936, 43.061684 });
    set_stage<Deco_Wood_Fall>({ -6.688574, -9.640275, 28.880888 });

    set_stage<Deco_Leaf_Fall>({11.538204, -9.620079, -45.287979} );
    set_stage<Deco_Leaf_Fall>({10.535198, -9.620079, -44.635147} );
    set_stage<Deco_Leaf_Fall>({-7.376638, -9.620079, -51.963013} );
    set_stage<Deco_Leaf_Fall>({-5.659158, -9.620079, -54.273129} );
    set_stage<Deco_Leaf_Fall>({-8.727056, -9.620079, -55.864609} );
    set_stage<Deco_Leaf_Fall>({10.135153, -9.620079, -54.644535} );
    set_stage<Deco_Kinoko>({8.804041, -9.620079, -26.985693}  );
    set_stage<Deco_Leaf_Fall>({10.645514, -9.620079, -28.461252} );
    set_stage<Deco_Leaf_Fall>({-8.521770, -9.620079, -28.735723} );
    set_stage<Deco_Leaf_Fall>({9.415770, -9.620079, 12.563226}   );
    set_stage<Deco_Leaf_Fall>({8.992554, -9.620079, 16.321186}   );
    set_stage<Deco_Kinoko>({6.112177, -9.620079, 21.677444}   );
    set_stage<Deco_Leaf_Fall>({4.191730, -9.620079, 26.162622}   );
    set_stage<Deco_Leaf_Fall>({6.422721, -9.620079, 35.657082}   );
    set_stage<Deco_Leaf_Fall>({-6.361845, -9.620079, 9.543242}   );
    set_stage<Deco_Pumpkin>({-9.422297, -9.620079, 12.481453}  );
    set_stage<Deco_Leaf_Fall>({-10.641525, -9.620079, 15.870753} );
    set_stage<Deco_Leaf_Fall>({-6.002773, -9.620079, 16.265963}  );
    set_stage<Deco_Kinoko>({-11.331450, -9.620079, 9.637025}  );
    set_stage<Deco_Leaf_Fall>({-14.658218, -9.620079, 51.743366} );
    set_stage<Deco_Kinoko>({-16.357674, -9.620079, 51.442451} );
    set_stage<Deco_Leaf_Fall>({-8.689487, -9.620079, 56.801075}  );
    set_stage<Deco_Pumpkin>({-6.320377, -9.620079, 51.431236}  );
    set_stage<Deco_Leaf_Fall>({-9.974609, -9.620079, 48.325588}  );
    set_stage<Deco_Leaf_Fall>({-8.659941, -9.620079, 41.389774}  );
    set_stage<Deco_Leaf_Fall>({-13.853856, -9.620079, 44.789131} );
    set_stage<Deco_Leaf_Fall>({8.247876, -9.620079, 40.169781}   );
    set_stage<Deco_Leaf_Fall>({13.861238, -9.620079, 43.867184}  );
    set_stage<Deco_Leaf_Fall>({11.019861, -9.620079, 48.444828}  );
    set_stage<Deco_Leaf_Fall>({18.411880, -9.620079, 50.147972}  );
    set_stage<Deco_Leaf_Fall>({14.955701, -9.620079, 53.910721}  );
    set_stage<Deco_Kinoko>({6.718028, -9.620079, 55.364906}   );
    set_stage<Deco_Leaf_Fall>({8.348392, -9.620079, 63.828358}   );
    set_stage<Deco_Leaf_Fall>({5.818618, -9.620079, 67.321182}   );
    set_stage<Deco_Leaf_Fall>({11.214822, -9.620079, 67.740395}  );
    set_stage<Deco_Pumpkin>({8.472459, -9.620079, 69.480553}   );
    set_stage<Deco_Leaf_Fall>({16.930576, -9.620079, 40.474590}  );
    set_stage<Deco_Leaf_Fall>({18.310558, -9.620079, 41.246696}  );
    set_stage<Deco_Kinoko>({-8.199827, -9.620079, 65.332520}  );
    set_stage<Deco_Leaf_Fall>({-5.002496, -9.620079, 32.681110}  );
    set_stage<Deco_Leaf_Fall>({-6.813102, -9.620079, 23.338760}  );
    set_stage<Deco_Leaf_Fall>({-8.756380, -9.620079, 19.685165}  );
    set_stage<Deco_Pumpkin>({-10.514501, -9.621490, -33.532784});
    set_stage<Deco_Leaf_Fall>({11.668956, -9.621490, -39.444893} );
    set_stage<Deco_Leaf_Fall>({7.137900, -9.621490, -33.356880}  );
    set_stage<Deco_Kinoko>({7.247359, -9.621490, -49.314617}  );
    set_stage<Deco_Leaf_Fall>({-11.967325, -9.621490, -26.313593});


}

void Stage_Spawner::set_stage_4()
{
    set_stage<Stage_4>({ 0.0f,0.0f,0.0f });
    set_stage<Stage_4_Move>({ 0.0f,1.0f,-67.0f })->set_start({ 0.0f,7.0f,-67.0f }).set_end({ 0.0f,0.0f,-67.0f }).set_scale(0.1f);
    set_stage<Stage_4_Movement>({ 0.0f,5.0f,-15.0f });
    set_stage<Stage_4_Movement>({ 0.0f,2.0f,14.0f });
    set_stage<Stage_4_Goal>({ 0.0f,0.0f,0.0f });

    set_stage<Deco_Wood_Winter>({-5.072042, 0.178309, -119.782204});
    set_stage<Deco_Wood_Winter>({-3.756578, 0.178309, -114.400703});
    set_stage<Deco_Wood_Winter>({4.977135, 0.178309, -112.077827} );
    set_stage<Deco_Wood_Winter>({-7.786924, -0.718330, -85.539757});
    set_stage<Deco_Wood_Winter>({7.803653, -0.718330, -90.968582} );
    set_stage<Deco_Wood_Winter>({-18.777504, 9.097606, 125.413315});
    set_stage<Deco_Wood_Winter>({0.029140, 9.097606, 129.701538}  );
    set_stage<Deco_Wood_Winter>({19.662209, 9.097606, 117.682915} );
    set_stage<Deco_Wood_Winter>({-18.226181, 9.097606, 103.934937});


    set_stage<Deco_Snow_Man>({4.882720, 0.256061, -120.899185}   );
    set_stage<Deco_Kamakura>({-7.569036, -0.642542, -97.019562}  );
    set_stage<Deco_Leaf_Winter>({6.299351, -0.642542, -87.905891}   );
    set_stage<Deco_Leaf_Winter>({6.299351, -0.642542, -95.151489}   );
    set_stage<Deco_Leaf_Winter>({-6.056017, -0.642542, -90.379997}  );
    set_stage<Deco_Leaf_Winter>({-8.126642, -0.642542, -93.047081}  );
    set_stage<Deco_Leaf_Winter>({4.043475, 0.258198, -107.259880}   );
    set_stage<Deco_Leaf_Winter>({-3.323265, 0.258198, -107.289406}  );
    set_stage<Deco_Leaf_Winter>({3.638399, 0.258198, -115.704414}   );
    set_stage<Deco_Leaf_Winter>({-5.701273, 0.258198, -111.070694}  );
    set_stage<Deco_Leaf_Winter>({4.918447, 6.629390, -38.285889}    );
    set_stage<Deco_Kamakura>({-10.260880, 9.106196, 122.436775}  )->set_scale(0.12f);
    set_stage<Deco_Leaf_Winter>({11.050152, 9.106196, 122.891891}   );
    set_stage<Deco_Leaf_Winter>({17.315588, 9.106196, 122.023224}   );
    set_stage<Deco_Leaf_Winter>({18.766335, 9.106196, 107.095192}   );
    set_stage<Deco_Leaf_Winter>({19.142611, 9.106196, 101.048607}   );
    set_stage<Deco_Leaf_Winter>({14.014485, 9.106196, 99.467514}    );
    set_stage<Deco_Leaf_Winter>({9.155117, 9.106196, 91.786064}     );
    set_stage<Deco_Leaf_Winter>({-8.983050, 9.106196, 92.220779}    );
    set_stage<Deco_Leaf_Winter>({-8.998720, 9.106196, 85.762650}    );
    set_stage<Deco_Snow_Man>({-3.628353, 9.106196, 83.865662}    );
    set_stage<Deco_Leaf_Winter>({6.436167, 9.106196, 86.127060}     );


}

void Stage_Spawner::set_stage_boss()
{
    set_stage<Stage_Boss>();
}