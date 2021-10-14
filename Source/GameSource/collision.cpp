

#include "collision.h"



bool Collision3D::BallVsBallAndExtrusion(
    const DirectX::XMFLOAT3& pivotA,
    const float radiusA_,
    const DirectX::XMFLOAT3& pivotB,
    const float radiusB_,
    DirectX::XMFLOAT3& outPosB )
{
    DirectX::XMVECTOR PivotA      = DirectX::XMLoadFloat3(&pivotA);
    DirectX::XMVECTOR PivotB      = DirectX::XMLoadFloat3(&pivotB);
    DirectX::XMVECTOR Vec       = DirectX::XMVectorSubtract(PivotA, PivotB);
    DirectX::XMVECTOR LengthSq  = DirectX::XMVector3LengthSq(Vec);
    float lengthsq;
    DirectX::XMStoreFloat(&lengthsq, LengthSq);

    float range = radiusA_ + radiusB_;
    if (range * range < lengthsq) return false;

    // ������Ȃ�range�������o��
    Vec = DirectX::XMVector3Normalize(Vec);
    DirectX::XMVECTOR outVec = DirectX::XMVectorScale(Vec, -1.0f * range); // �������ĂĂ�t�x�N�g���ɂ���
    DirectX::XMStoreFloat3(&outPosB, DirectX::XMVectorAdd(PivotA, outVec));

    // position.y�́@pivot�̔��a�����ɂ���̂�
    outPosB.y += -radiusB_;

    return true;
}

// ���Ƌ��̌�������
bool Collision3D::IntersectSqhereVsSqhere(
    const DirectX::XMFLOAT3& positionA,
    float radiusA,
    const DirectX::XMFLOAT3& positionB,
    float radiusB,
    DirectX::XMFLOAT3& outPositionB)
{
    // A->B
    DirectX::XMVECTOR PositionA = DirectX::XMLoadFloat3(&positionA);
    DirectX::XMVECTOR PositionB = DirectX::XMLoadFloat3(&positionB);

    //DirectX::XMVECTOR Vec		= DirectX::XMVectorSubtract(PositionB, PositionA);
    using namespace DirectX;
    DirectX::XMVECTOR Vec = PositionB - PositionA;

    DirectX::XMVECTOR LengthSq = DirectX::XMVector3LengthSq(Vec);
    float lengthSq;
    DirectX::XMStoreFloat(&lengthSq, LengthSq);

    // ��������
    float range = radiusA + radiusB;
    if (lengthSq > range * range)
    {
        return false; // �������Ă��Ȃ�
    }

    // A��B�������o��
    //Vec /= LengthSq;
    Vec = DirectX::XMVector3Normalize(Vec);
    Vec *= range;// ����range��A��B�x�N�g��
    PositionB = PositionA + Vec;

    DirectX::XMStoreFloat3(&outPositionB, PositionB);

    return true; // �������Ă���
}

// �~���Ɖ~���̌�������
bool Collision3D::IntersectCylinderVsCylinder(
    const DirectX::XMFLOAT3& positionA,
    float radiusA,
    float heightA,
    const DirectX::XMFLOAT3& positionB,
    float radiusB,
    float heightB,
    DirectX::XMFLOAT3& outPositionB)
{
    // A�̑�����B�̓�����Ȃ瓖�����Ă��Ȃ�
    if (positionA.y > positionB.y + heightB)
    {
        return false;
    }
    // A�̓���B�̑�����艺�Ȃ瓖�����Ă��Ȃ�
    if (positionA.y + heightA < positionB.y)
    {
        return false;
    }

    //XZ���ʂł͈̔̓`�F�b�N
    float vx = positionB.x - positionA.x;
    float vz = positionB.z - positionA.z;
    float dist = sqrtf(vx * vx + vz * vz);
    float range = radiusA + radiusB;

    if (dist > range)
    {
        return false; // �������Ă��Ȃ�
    }

    // A��B�������o��
    vx /= dist;
    vz /= dist;

    outPositionB.x = positionA.x + vx * range;
    outPositionB.y = positionB.y;
    outPositionB.z = positionA.z + vz * range;

    return true;
}

// ���Ɖ~���̌�������
bool Collision3D::IntersectSqhereVsCylinder(
    const DirectX::XMFLOAT3& sqherePosition,
    float sqhereRadius,
    const DirectX::XMFLOAT3& cylinderPosition,
    float cylinderRadius,
    float cylinderHeight,
    DirectX::XMFLOAT3& outCylinderPosition
)
{
    // A�̑�����B�̓�����Ȃ瓖�����Ă��Ȃ�
    if (sqherePosition.y > cylinderPosition.y + cylinderHeight)
    {
        return false;
    }
    // A�̓���B�̑�����艺�Ȃ瓖�����Ă��Ȃ�
    if (sqherePosition.y + sqhereRadius < cylinderPosition.y)
    {
        return false;
    }

    //XZ���ʂł͈̔̓`�F�b�N
    float vx = cylinderPosition.x - sqherePosition.x;
    float vz = cylinderPosition.z - sqherePosition.z;
    float dist = sqrtf(vx * vx + vz * vz);
    float range = sqhereRadius + cylinderRadius;

    if (dist > range)
    {
        return false; // �������Ă��Ȃ�
    }

    // A��B�������o��
    vx /= dist;
    vz /= dist;

    outCylinderPosition.x = sqherePosition.x + vx * range;
    outCylinderPosition.y = cylinderPosition.y;
    outCylinderPosition.z = sqherePosition.z + vz * range;

    return true;
}

bool Collision3D::RayPickVsModel(
    const DirectX::XMFLOAT3& start,
    const DirectX::XMFLOAT3& end,
    const Model* model,
    HitResult& result
)
{
    // �ȑO�̒n�ʏ���
    /*if (end.y < 0.0f)
    {
        result.position.x = end.x;
        result.position.y = 0.0f;
        result.position.z = end.z;
        result.normal.x = 0.0f;
        result.normal.y = 1.0f;
        result.normal.z = 0.0f;
        return true;
    }*/


    DirectX::XMVECTOR WorldStart = DirectX::XMLoadFloat3(&start);
    DirectX::XMVECTOR WorldEnd = DirectX::XMLoadFloat3(&end);
    DirectX::XMVECTOR WorldRayVec = DirectX::XMVectorSubtract(WorldEnd, WorldStart);
    DirectX::XMVECTOR WorldRayLength = DirectX::XMVector3Length(WorldRayVec);

    DirectX::XMStoreFloat(&result.distance, WorldRayLength);

    bool hit = false;
    const ModelResource* resource = model->GetResource();
    for (const ModelResource::Mesh& mesh : resource->GetMeshes()) // ���f���Ƀ��b�V�����������????
    {
        const Model::Node& node = model->GetNodes().at(mesh.nodeIndex);// ���b�V���̃m�[�h�����擾?

        DirectX::XMMATRIX WorldTransform = DirectX::XMLoadFloat4x4(&node.worldTransform);
        DirectX::XMMATRIX InverseWorldTransform = DirectX::XMMatrixInverse(nullptr, WorldTransform);

        DirectX::XMVECTOR Start = DirectX::XMVector3TransformCoord(WorldStart, InverseWorldTransform);
        DirectX::XMVECTOR End = DirectX::XMVector3TransformCoord(WorldEnd, InverseWorldTransform);
        DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(End, Start);
        const DirectX::XMVECTOR Dir = DirectX::XMVector3Normalize(Vec);
        DirectX::XMVECTOR Length = DirectX::XMVector3Length(Vec);




        // ���C�̒���
        float neart;
        DirectX::XMStoreFloat(&neart, Length);

        // --- �O�p�` --- //
        // ���_���
        const std::vector<ModelResource::Vertex>& vertices = mesh.vertices;
        // �|���S��
        const std::vector <UINT>& indices = mesh.indices;

        int meterialIndex = -1;
        DirectX::XMVECTOR HitPosition;
        DirectX::XMVECTOR HitNormal;

        // ���b�V���̃T�u�Z�b�g�œ����蔻������
        for (const ModelResource::Subset& subset : mesh.subsets)
        {
            // �|���S�����Ƃɔ���
            for (UINT i = 0; i < subset.indexCount; i += 3)
            {
                UINT index = subset.startIndex + i;

                const ModelResource::Vertex& a = vertices.at(indices.at(index));
                const ModelResource::Vertex& b = vertices.at(indices.at(index + 1));
                const ModelResource::Vertex& c = vertices.at(indices.at(index + 2));

                DirectX::XMVECTOR A = DirectX::XMLoadFloat3(&a.position);
                DirectX::XMVECTOR B = DirectX::XMLoadFloat3(&b.position);
                DirectX::XMVECTOR C = DirectX::XMLoadFloat3(&c.position);

                // �O�p�`�̎O�Ӄx�N�g���Z�o

                DirectX::XMVECTOR AB = DirectX::XMVectorSubtract(B, A);
                DirectX::XMVECTOR BC = DirectX::XMVectorSubtract(C, B);
                DirectX::XMVECTOR CA = DirectX::XMVectorSubtract(A, C);

                // �O�ςŖ@�����Z�o
                DirectX::XMVECTOR Normal = DirectX::XMVector3Cross(AB, BC);

                // �O�ς�0�ɂȂ��Ă���Ƃ�
                DirectX::XMFLOAT3 normalized;
                DirectX::XMStoreFloat3(&normalized, Normal);
                if (normalized.x == 0.0f &&
                    normalized.y == 0.0f &&
                    normalized.z == 0.0f)
                {
                    DirectX::XMFLOAT3 bc;
                    DirectX::XMStoreFloat3(&bc, BC);
                    bc.x += 0.001f;
                    bc.y += 0.0f; // ���₷�K�v�Ȃ�
                    bc.z += 0.0f; // ���₷�K�v�Ȃ�
                    Normal = DirectX::XMVector3Cross(AB, DirectX::XMVectorSet(bc.x, bc.y, bc.z, 1.0f));
                }

                // ���ς̌��ʂ��v���X�Ȃ痠���� //�s�p�Ȃ痠�����A�݊p�Ȃ�\����
                DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(Normal, Dir);
                float dot;
                DirectX::XMStoreFloat(&dot, Dot);
                if (dot > 0.0f) continue;


                // ��_���Z�o
                // �ˉe1
                DirectX::XMVECTOR V = DirectX::XMVectorSubtract(A, Start);


                //// �ŏ�
                //DirectX::XMVECTOR T = DirectX::XMVector3Dot(V, Dir); 

                    // ��y
                DirectX::XMVECTOR T = DirectX::XMVectorDivide(DirectX::XMVector3Dot(Normal, V), Dot);

                float t;
                DirectX::XMStoreFloat(&t, T);
                //t = t / (dot + t);
                if (t < .0f || t > neart) continue;

                // ��_
                /*DirectX::XMVECTOR SUM = DirectX::XMVectorAdd( DirectX::XMVectorSet(dot, dot, dot, dot), DirectX::XMVectorSet(t, t, t, t) );
                DirectX::XMVECTOR X = DirectX::XMVectorDivide( DirectX::XMVectorSet(t, t, t, t), SUM );*/

                //DirectX::XMStoreFloat(&x, X);
                DirectX::XMVECTOR Position = DirectX::XMVectorAdd(Start, DirectX::XMVectorScale(Dir, t));


                // ��_���O�p�`�̓����ɂ��邩���� 
                // 1
                DirectX::XMVECTOR V1 = DirectX::XMVectorSubtract(Position, A);
                DirectX::XMVECTOR Cross1 = DirectX::XMVector3Cross(AB, V1);
                DirectX::XMVECTOR Dot1 = DirectX::XMVector3Dot(Cross1, Normal);
                float dot1;
                DirectX::XMStoreFloat(&dot1, Dot1);
                if (dot1 < 0.0f) continue;

                // 2
                DirectX::XMVECTOR V2 = DirectX::XMVectorSubtract(Position, B);
                DirectX::XMVECTOR Cross2 = DirectX::XMVector3Cross(BC, V2);
                DirectX::XMVECTOR Dot2 = DirectX::XMVector3Dot(Cross2, Normal);
                float dot2;
                DirectX::XMStoreFloat(&dot2, Dot2);
                if (dot2 < 0.0f) continue;

                // 3
                DirectX::XMVECTOR V3 = DirectX::XMVectorSubtract(Position, C);
                DirectX::XMVECTOR Cross3 = DirectX::XMVector3Cross(CA, V3);
                DirectX::XMVECTOR Dot3 = DirectX::XMVector3Dot(Cross3, Normal);
                float dot3;
                DirectX::XMStoreFloat(&dot3, Dot3);
                if (dot3 < 0.0f) continue;

                // �ŋߋ������X�V
                neart = t;

                HitPosition = Position;
                HitNormal = Normal;
                meterialIndex = subset.materialIndex;


            }

        }

        // �}�e���A���ԍ��������
        if (meterialIndex >= 0)
        {
            // ���[�J����Ԃ��烏�[���h��Ԃ֕ϊ�
            DirectX::XMVECTOR WorldPosition = DirectX::XMVector3TransformCoord(HitPosition, WorldTransform);
            DirectX::XMVECTOR WorldCrossVec = DirectX::XMVectorSubtract(WorldPosition, WorldStart);
            DirectX::XMVECTOR WorldCrossLength = DirectX::XMVector3Length(WorldCrossVec);
            float distance;
            DirectX::XMStoreFloat(&distance, WorldCrossLength);

            // �q�b�g���ۑ�
            if (result.distance > distance)
            {
                DirectX::XMVECTOR WorldNormal = DirectX::XMVector3TransformNormal(HitNormal, WorldTransform);

                result.distance = distance;
                result.materialIndex = meterialIndex;
                DirectX::XMStoreFloat3(&result.position, WorldPosition);
                DirectX::XMStoreFloat3(&result.normal, DirectX::XMVector3Normalize(WorldNormal));
                hit = true;
            }


        }


    }


    return hit;
}


bool Collision2D::CircleVsCircle(
    const DirectX::XMFLOAT2& pos_A,
    const float radius_A,
    const DirectX::XMFLOAT2& pos_B,
    const float radius_B)
{
    float length = 0.0f;
    DirectX::XMStoreFloat(&length, DirectX::XMVector2Length(DirectX::XMVectorSet(pos_B.x - pos_A.x, pos_B.y - pos_A.y, 0.0f, 1.0f)));

    return (length > radius_A + radius_B);
}


bool Collision2D::CircleVsCircleAndExtrusion(
    const DirectX::XMFLOAT2& pos_A,
    const float radius_A,
    const DirectX::XMFLOAT2& pos_B,
    const float radius_B,
    DirectX::XMFLOAT2& out_pos_B)
{
    DirectX::XMVECTOR AtoB_vec = DirectX::XMVectorSubtract(DirectX::XMLoadFloat2(&pos_B), DirectX::XMLoadFloat2(&pos_A));
    float length = 0.0f;
    DirectX::XMStoreFloat(&length, DirectX::XMVector2Length(AtoB_vec));

    if (length < radius_A + radius_B) return false;


    float subtract = radius_A + radius_B - length;

    DirectX::XMStoreFloat2(&out_pos_B, DirectX::XMVectorAdd(DirectX::XMLoadFloat2(&pos_B), DirectX::XMVectorScale(DirectX::XMVector2Normalize(AtoB_vec), subtract)));


    return true;
}


bool Collision2D::RectVsRect(
    const DirectX::XMFLOAT2& rect_A_center_pos,
    const DirectX::XMFLOAT2& rect_A_size,
    const DirectX::XMFLOAT2& rect_B_center_pos,
    const DirectX::XMFLOAT2& rect_B_size)
{
    float rect_A_half_size_width = rect_A_size.x * 0.5f;
    float rect_A_half_size_height = rect_A_size.y * 0.5f;

    DirectX::XMFLOAT2 A_min = { rect_A_center_pos.x - rect_A_half_size_width, rect_A_center_pos.y - rect_A_half_size_height };
    DirectX::XMFLOAT2 A_max = { rect_A_center_pos.x + rect_A_half_size_width, rect_A_center_pos.y + rect_A_half_size_height };


    float rect_B_half_size_width = rect_B_size.x * 0.5f;
    float rect_B_half_size_height = rect_B_size.y * 0.5f;

    DirectX::XMFLOAT2 B_min = { rect_B_center_pos.x - rect_B_half_size_width, rect_B_center_pos.y - rect_B_half_size_height };
    DirectX::XMFLOAT2 B_max = { rect_B_center_pos.x + rect_B_half_size_width, rect_B_center_pos.y + rect_B_half_size_height };


    if (A_min.x > B_max.x) return false;
    if (A_max.x < B_min.x) return false;
    if (A_min.y > B_max.y) return false;
    if (A_max.y < B_min.y) return false;

    return true;
}


bool Collision2D::RectVsRectAndExtrusion(
    const DirectX::XMFLOAT2& rect_A_center_pos,
    const DirectX::XMFLOAT2& rect_A_size,
    const DirectX::XMFLOAT2& rect_B_center_pos,
    const DirectX::XMFLOAT2& rect_B_size,
    DirectX::XMFLOAT2& rect_A_out_center_pos)
{
    
    /*// �_
    DirectX::XMFLOAT2 point_position = rect_B_center_pos;

    // ��`�̍ŏ��_�ƍő�_
    DirectX::XMFLOAT2 min = { rect_A_center_pos.x - rect_A_size.x * 0.5f - rect_B_size.x * 0.5f, rect_A_center_pos.y - rect_A_size.y * 0.5f - rect_B_size.x * 0.5f };
    DirectX::XMFLOAT2 max = { rect_A_center_pos.x + rect_A_size.x * 0.5f + rect_B_size.x * 0.5f, rect_A_center_pos.y + rect_A_size.y * 0.5f + rect_B_size.x * 0.5f };


    auto ClampOnRange = [](const float num, const float min, const float max) -> float
    {
        if (num < min) return min;
        if (num > max) return max;

        return num;
    };

    // ��`���ŉ~�̒��S�_�ɍł��߂��_���Z�o����
    DirectX::XMFLOAT2 rect_point_pos = {};
    rect_point_pos.x = ClampOnRange(point_position.x, min.x, max.x);
    rect_point_pos.y = ClampOnRange(point_position.y, min.y, max.y);

    // �ŋߓ_�Ɖ~�ŏՓ˔��������
    

    // �Փ˂��Ă�����... //

    

    // �߂荞��ł鋗�����v�Z

    // �߂荞��ł��镪�����o������x�N�g������
    DirectX::XMFLOAT2 n_vec;

    rect_B_out_center_pos = { rect_B_center_pos.x - n_vec.x * length, rect_B_center_pos.y - n_vec.y * length };

    return true;*/

    bool isHit = false;

	/*if (rect_A_center_pos.x - rect_A_size.x <= rect_B_center_pos.x - rect_B_size.x)
	{
        rect_A_out_center_pos.x = rect_B_center_pos.x - rect_B_size.x + rect_A_size.x;
		isHit = true;
	}
	if (rect_B_center_pos.x + rect_B_size.x <= rect_A_center_pos.x + rect_A_size.x)
	{
        rect_A_out_center_pos.x = rect_B_center_pos.x + rect_B_size.x - rect_A_size.x;
		isHit = true;
	}
	if (rect_A_center_pos.y - rect_A_size.y <= rect_B_center_pos.y - rect_B_size.y)
	{
        rect_A_out_center_pos.y = rect_B_center_pos.y - rect_B_size.y + rect_A_size.y;
		isHit = true;
	}
	if (rect_B_center_pos.y + rect_B_size.y <= rect_A_center_pos.y + rect_A_size.y)
	{
        rect_A_out_center_pos.y = rect_B_center_pos.y + rect_B_size.y - rect_A_size.y;
		isHit = true;
	}*/

	return isHit;
}


bool Collision2D::RectVsCircleAndExtrusion(DirectX::XMFLOAT2 rect_center_position, DirectX::XMFLOAT2 rect_size, DirectX::XMFLOAT2 circle_center_position, float circle_radius, DirectX::XMFLOAT2& circle_out_center_position)
{
    auto ClampOnRange = [](const float num, const float min, const float max) -> float
    {
        if (num < min) return min;
        if (num > max) return max;

        return num;
    };
    // ��`�̍ŏ��_�ƍő�_
    DirectX::XMFLOAT2 min = { rect_center_position.x - rect_size.x * 0.5f, rect_center_position.y - rect_size.y * 0.5f };
    DirectX::XMFLOAT2 max = { rect_center_position.x + rect_size.x * 0.5f, rect_center_position.y + rect_size.y * 0.5f };


    // ��`���ŉ~�̒��S�_�ɍł��߂��_���Z�o����
    DirectX::XMFLOAT2 rect_point_pos = {};
    rect_point_pos.x = ClampOnRange(circle_center_position.x, min.x, max.x);
    rect_point_pos.y = ClampOnRange(circle_center_position.y, min.y, max.y);

    // �ŋߓ_�Ɖ~�ŏՓ˔��������
    DirectX::XMVECTOR vec_circle_to_rect_point = DirectX::XMLoadFloat2(&DirectX::XMFLOAT2({ rect_point_pos.x - circle_center_position.x,rect_point_pos.y - circle_center_position.y }));
    float length;
    DirectX::XMStoreFloat(&length, DirectX::XMVector2Length(vec_circle_to_rect_point));

    if (circle_radius <= length)
    {
        // �Փ˂��Ă��Ȃ�
        return false;
    }

    // �Փ˂��Ă�����... //

    // ��`�̓����܂ł߂荞��ł��܂��Ă�����
    // """""""""""""""""""""""""""""""""����**�G**""""""""""""""""""""""""""""""
    if (length <= 0.0f)
    {
        float temp_length[2] = {};
        DirectX::XMVECTOR vec = DirectX::XMLoadFloat2(&DirectX::XMFLOAT2({ min.x - circle_center_position.x, min.y - circle_center_position.y }));
        DirectX::XMStoreFloat(&temp_length[0], DirectX::XMVector2Length(vec));

        vec = DirectX::XMLoadFloat2(&DirectX::XMFLOAT2({ max.x - circle_center_position.x, max.y - circle_center_position.y }));
        DirectX::XMStoreFloat(&temp_length[1], DirectX::XMVector2Length(vec));

        if (temp_length[0] < temp_length[1])
        {
            length = temp_length[0];
            vec_circle_to_rect_point = { min.x - circle_center_position.x, min.y - circle_center_position.y };
        }

        else
        {
            length = temp_length[1];
            vec_circle_to_rect_point = { max.x - circle_center_position.x, max.y - circle_center_position.y };
        }
    }

    // �߂荞��ł鋗�����v�Z
    length = circle_radius - length;

    // �߂荞��ł��镪�����o������x�N�g������
    DirectX::XMFLOAT2 n_vec;
    DirectX::XMStoreFloat2(&n_vec, DirectX::XMVector2Normalize(vec_circle_to_rect_point));

    circle_out_center_position = { circle_center_position.x - n_vec.x * length, circle_center_position.y - n_vec.y * length };

    return true;
}