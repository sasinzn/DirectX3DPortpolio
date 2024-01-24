#include "Framework.h"

Camera::Camera()
{
    viewBuffer = new ViewBuffer();
    viewBuffer->SetVS(1);
}

Camera::~Camera()
{
    delete viewBuffer;
}

void Camera::Update()
{
    projection = Environment::Get()->GetProjection();
    Frustum();

    if (target)
        FollowMode();
    else
        FreeMode();

    UpdateWorld();
}

void Camera::GUIRender()
{
    if (ImGui::TreeNode("Camera Option"))
    {
        ImGui::SliderFloat("MoveSpeed", &moveSpeed, 1.0f, 200.0f);
        ImGui::SliderFloat("RotSpeed", &rotSpeed, 1.0f, 50.0f);

        if (target && ImGui::TreeNode("TargetOption"))
        {
            ImGui::DragFloat("Distance", &distance, 0.1f);
            ImGui::DragFloat("Height", &height, 0.1f);
            ImGui::DragFloat3("FocusOffset", (float*)&focusOffset, 0.1f);

            float degree = XMConvertToDegrees(rotY);
            ImGui::DragFloat("RotY", &degree, 0.1f);
            rotY = XMConvertToRadians(degree);

            ImGui::DragFloat("MoveDamping", &moveDamping, 0.1f);
            ImGui::DragFloat("RotDamping", &rotDamping, 0.1f);

            ImGui::Checkbox("LookAtTargetX", &isLookAtTargetX);
            ImGui::Checkbox("LookAtTargetY", &isLookAtTargetY);

            ImGui::InputText("File", file, 128);

            if (ImGui::Button("Save"))
                TargetOptionSave(file);

            ImGui::SameLine();

            if (ImGui::Button("Load"))
                TargetOptionLoad(file);

            ImGui::TreePop();
        }

        Transform::GUIRender();

        ImGui::TreePop();
    }
}

void Camera::FreeMode()
{
    Vector3 delta = mousePos - prevMousePos;
    prevMousePos = mousePos;

    if (KEY->Press(VK_RBUTTON) && topView == false)
    {
        if (KEY->Press('W'))
            Translate(GetForward() * moveSpeed * DELTA);
        if (KEY->Press('S'))
            Translate(GetBack() * moveSpeed * DELTA);
        if (KEY->Press('A'))
            Translate(GetLeft() * moveSpeed * DELTA);
        if (KEY->Press('D'))
            Translate(GetRight() * moveSpeed * DELTA);
        if (KEY->Press('Q'))
            Translate(GetUp() * moveSpeed * DELTA);
        if (KEY->Press('E'))
            Translate(GetDown() * moveSpeed * DELTA);

        Rotate(Vector3::Up() * delta.x * DELTA);
        Rotate(Vector3::Left() * delta.y * DELTA);
    }

    if (topView)
    {
        if (KEY->Press('W'))
            Translate(GetUp() * moveSpeed * DELTA);
        if (KEY->Press('S'))
            Translate(GetDown() * moveSpeed * DELTA);
        if (KEY->Press('A'))
            Translate(GetLeft() * moveSpeed * DELTA);
        if (KEY->Press('D'))
            Translate(GetRight() * moveSpeed * DELTA);
          
    }
}

void Camera::FollowMode()
{
    destRot = Lerp(destRot, target->GetLocalRotation().y, rotDamping * DELTA);
    rotMatrix = XMMatrixRotationY(destRot + rotY);

    Vector3 forward = XMVector3TransformNormal(Vector3::Back(), rotMatrix);

    destPos = target->GetGlobalPosition() + forward * distance;
    destPos.y += height;

    localPosition = Lerp(localPosition, destPos, moveDamping * DELTA);

    Vector3 offset = XMVector3TransformCoord(focusOffset, rotMatrix);
    Vector3 targetPos = target->GetGlobalPosition() + offset;

    Vector3 dir = (targetPos - localPosition).GetNormalized();
    forward = Vector3(dir.x, 0.0f, dir.z).GetNormalized();

    if (isLookAtTargetX)
        localRotation.x = acos(Dot(forward, dir));
    if (isLookAtTargetY)
        localRotation.y = atan2(dir.x, dir.z);
}

void Camera::Frustum()
{
    Float4x4 VP;
    XMStoreFloat4x4(&VP, view * projection);

    //Left
    a = VP._14 + VP._11;
    b = VP._24 + VP._21;
    c = VP._34 + VP._31;
    d = VP._44 + VP._41;
    planes[0] = XMVectorSet(a, b, c, d);

    //Right
    a = VP._14 - VP._11;
    b = VP._24 - VP._21;
    c = VP._34 - VP._31;
    d = VP._44 - VP._41;
    planes[1] = XMVectorSet(a, b, c, d);

    //Bottom
    a = VP._14 + VP._12;
    b = VP._24 + VP._22;
    c = VP._34 + VP._32;
    d = VP._44 + VP._42;
    planes[2] = XMVectorSet(a, b, c, d);

    //Top
    a = VP._14 - VP._12;
    b = VP._24 - VP._22;
    c = VP._34 - VP._32;
    d = VP._44 - VP._42;
    planes[3] = XMVectorSet(a, b, c, d);

    //Near
    a = VP._14 + VP._13;
    b = VP._24 + VP._23;
    c = VP._34 + VP._33;
    d = VP._44 + VP._43;
    planes[4] = XMVectorSet(a, b, c, d);

    //Far
    a = VP._14 - VP._13;
    b = VP._24 - VP._23;
    c = VP._34 - VP._33;
    d = VP._44 - VP._43;
    planes[5] = XMVectorSet(a, b, c, d);

    FOR(6)
        planes[i] = XMPlaneNormalize(planes[i]);
}

void Camera::SetView()
{
    view = XMMatrixInverse(nullptr, world);
    viewBuffer->Set(view, world);

    viewBuffer->SetVS(1);
    viewBuffer->SetPS(1);
}

Ray Camera::ScreenPointToRay(Vector3 screenPoint)
{
    Vector3 screenSize(WIN_WIDTH, WIN_HEIGHT, 1.0f);

    Float2 point;
    point.x = (screenPoint.x / screenSize.x) * 2.0f - 1.0f;
    point.y = (screenPoint.y / screenSize.y) * 2.0f - 1.0f;

    Float4x4 temp;
    XMStoreFloat4x4(&temp, projection);

    screenPoint.x = point.x / temp._11;
    screenPoint.y = point.y / temp._22;
    screenPoint.z = 1.0f;

    screenPoint = XMVector3TransformNormal(screenPoint, world);

    return Ray(localPosition, screenPoint.GetNormalized());
}

Vector3 Camera::WorldToScreen(Vector3 worldPos)
{
    Vector3 screenPos;

    screenPos = XMVector3TransformCoord(worldPos, view);
    screenPos = XMVector3TransformCoord(screenPos, projection);
    //NDC : -1 ~ 1

    screenPos = (screenPos + Vector3::One()) * 0.5f;//0~1

    screenPos.x *= WIN_WIDTH;
    screenPos.y *= WIN_HEIGHT;

    return screenPos;
}

void Camera::TargetOptionSave(string file)
{
    string path = "TextData/Camera/" + file + ".cam";

    BinaryWriter* writer = new BinaryWriter(path);

    writer->Float(distance);
    writer->Float(height);
    writer->Float(moveDamping);
    writer->Float(rotDamping);
    writer->Float(rotY);
    writer->Vector(focusOffset);
    writer->Bool(isLookAtTargetX);
    writer->Bool(isLookAtTargetY);

    delete writer;
}

void Camera::TargetOptionLoad(string file)
{
    string path = "TextData/Camera/" + file + ".cam";

    BinaryReader* reader = new BinaryReader(path);

    if (reader->IsFailed()) return;

    distance = reader->Float();
    height = reader->Float();
    moveDamping = reader->Float();
    rotDamping = reader->Float();
    rotY = reader->Float();
    focusOffset = reader->Vector();
    isLookAtTargetX = reader->Bool();
    isLookAtTargetY = reader->Bool();

    delete reader;
}

bool Camera::ContainPoint(Vector3 point)
{
    FOR(6)
    {
        Vector3 dot = XMPlaneDotCoord(planes[i], point);

        if (dot.x < 0.0f)
            return false;
    }

    return true;
}

bool Camera::ContainSphere(Vector3 center, float radius)
{
    Vector3 edge;
    Vector3 dot;

    FOR(6)
    {
        //1
        edge.x = center.x - radius;
        edge.y = center.y - radius;
        edge.z = center.z - radius;
        dot = XMPlaneDotCoord(planes[i], edge);
        if (dot.x > 0.0f)
            continue;

        //2
        edge.x = center.x + radius;
        edge.y = center.y - radius;
        edge.z = center.z - radius;
        dot = XMPlaneDotCoord(planes[i], edge);
        if (dot.x > 0.0f)
            continue;

        //3
        edge.x = center.x - radius;
        edge.y = center.y + radius;
        edge.z = center.z - radius;
        dot = XMPlaneDotCoord(planes[i], edge);
        if (dot.x > 0.0f)
            continue;

        //4
        edge.x = center.x - radius;
        edge.y = center.y - radius;
        edge.z = center.z + radius;
        dot = XMPlaneDotCoord(planes[i], edge);
        if (dot.x > 0.0f)
            continue;

        //5
        edge.x = center.x + radius;
        edge.y = center.y + radius;
        edge.z = center.z - radius;
        dot = XMPlaneDotCoord(planes[i], edge);
        if (dot.x > 0.0f)
            continue;

        //6
        edge.x = center.x + radius;
        edge.y = center.y - radius;
        edge.z = center.z + radius;
        dot = XMPlaneDotCoord(planes[i], edge);
        if (dot.x > 0.0f)
            continue;

        //7
        edge.x = center.x - radius;
        edge.y = center.y + radius;
        edge.z = center.z + radius;
        dot = XMPlaneDotCoord(planes[i], edge);
        if (dot.x > 0.0f)
            continue;

        //8
        edge.x = center.x + radius;
        edge.y = center.y + radius;
        edge.z = center.z + radius;
        dot = XMPlaneDotCoord(planes[i], edge);
        if (dot.x > 0.0f)
            continue;

        return false;
    }

    return true;
}

void Camera::SetTopView()
{
    Load("TopView");
    topView = true;
}
