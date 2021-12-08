
struct VS_OUT
{
    float3 local_position : POSITION;
    float4 world_position : SV_POSITION;
};


cbuffer SCENE_CONSTANT_BUFFER : register(b1)
{
    row_major float4x4 view_projection;	    //�r���[�E�v���W�F�N�V�����ϊ��s��
    row_major float4x4 view_rotate;         //�r���[�̉�]�s��
    float4 light_direction;		            //���C�g�̌���
    float4 light_radiance;		            //���C�g�̌���
    float4 camera_position;		            //�J�����ʒu
};