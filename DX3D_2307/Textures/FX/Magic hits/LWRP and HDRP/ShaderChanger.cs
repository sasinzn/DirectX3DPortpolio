using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;
using System.IO;

public class RPChanger : EditorWindow
{

    [MenuItem("Window/EGA-VFX Pipeline changer")]
    public static void ShowWindow()
    {
        EditorWindow.GetWindow(typeof(RPChanger));
    }

    public void OnGUI()
    {
        GUILayout.Label("Change pipeline to:");

        if (GUILayout.Button("Standard RP"))
        {
            FindShaders();
            ChangeToSRP();
        }
        if (GUILayout.Button("Lightweight RP"))
        {
            FindShaders();
            ChangeToLWRP();
        }
        if (GUILayout.Button("HD RP (From Unity 2018.3+)"))
        {
            FindShaders();
            ChangeToHDRP();
        }
    }

    Shader Add_CenterGlow;
    Shader Blend_CenterGlow;
    Shader Add_DistortTexture;
    Shader Blend_DistortTexture;
    Shader Add_Trail;
    Shader Blend_Electricity;
    Shader Blend_LitGlow;
    Shader Lit_CenterGlow;
    Shader Blend_TwoSides;
    Shader SmoothSmoke;
    Shader Blend_Normals;

    Shader Add_CenterGlow_LWRP; 
    Shader Blend_CenterGlow_LWRP;
    Shader Add_DistortTexture_LWRP;
    Shader Blend_DistortTexture_LWRP;
    Shader Add_Trail_LWRP;
    Shader Blend_Electricity_LWRP;
    Shader Blend_LitGlow_LWRP;
    Shader Lit_CenterGlow_LWRP;
    Shader Blend_TwoSides_LWRP;
    Shader SmoothSmoke_LWRP;
    Shader Blend_Normals_LWRP;

    Shader Add_CenterGlow_HDRP;
    Shader Blend_CenterGlow_HDRP;
    Shader Add_DistortTexture_HDRP;
    Shader Blend_DistortTexture_HDRP;
    Shader Add_Trail_HDRP;
    Shader Blend_Electricity_HDRP;
    Shader Blend_LitGlow_HDRP;
    Shader Lit_CenterGlow_HDRP;
    Shader Blend_TwoSides_HDRP;
    Shader SmoothSmoke_HDRP;
    Shader Blend_Normals_HDRP;

    Material[] shaderMaterials;

    private void FindShaders()
    {
        if (Shader.Find("EGA/Particles/Add_CenterGlow") != null) Add_CenterGlow = Shader.Find("EGA/Particles/Add_CenterGlow");
        if (Shader.Find("EGA/Particles/Blend_CenterGlow") != null) Blend_CenterGlow = Shader.Find("EGA/Particles/Blend_CenterGlow");
        if (Shader.Find("EGA/Particles/Add_DistortTexture") != null) Add_DistortTexture = Shader.Find("EGA/Particles/Add_DistortTexture");
        if (Shader.Find("EGA/Particles/Blend_DistortTexture") != null) Blend_DistortTexture = Shader.Find("EGA/Particles/Blend_DistortTexture");
        if (Shader.Find("EGA/Particles/Add_Trail") != null) Add_Trail = Shader.Find("EGA/Particles/Add_Trail");
        if (Shader.Find("EGA/Particles/Blend_Electricity") != null) Blend_Electricity = Shader.Find("EGA/Particles/Blend_Electricity");
        if (Shader.Find("EGA/Particles/Blend_LitGlow") != null) Blend_LitGlow = Shader.Find("EGA/Particles/Blend_LitGlow");
        if (Shader.Find("EGA/Particles/Lit_CenterGlow") != null) Lit_CenterGlow = Shader.Find("EGA/Particles/Lit_CenterGlow");
        if (Shader.Find("EGA/Particles/Blend_TwoSides") != null) Blend_TwoSides = Shader.Find("EGA/Particles/Blend_TwoSides");
        if (Shader.Find("EGA/Particles/SmoothSmoke") != null) SmoothSmoke = Shader.Find("EGA/Particles/SmoothSmoke");
        if (Shader.Find("EGA/Particles/Blend_Normals") != null) Blend_Normals = Shader.Find("EGA/Particles/Blend_Normals");

        if (Shader.Find("ErbGameArt/LWRP/Particles/Add_CenterGlow") != null) Add_CenterGlow_LWRP = Shader.Find("ErbGameArt/LWRP/Particles/Add_CenterGlow");
        if (Shader.Find("ErbGameArt/LWRP/Particles/Blend_CenterGlow") != null) Blend_CenterGlow_LWRP = Shader.Find("ErbGameArt/LWRP/Particles/Blend_CenterGlow");
        if (Shader.Find("ErbGameArt/LWRP/Particles/Add_DistortTexture") != null) Add_DistortTexture_LWRP = Shader.Find("ErbGameArt/LWRP/Particles/Add_DistortTexture");
        if (Shader.Find("ErbGameArt/LWRP/Particles/Blend_DistortTexture") != null) Blend_DistortTexture_LWRP = Shader.Find("ErbGameArt/LWRP/Particles/Blend_DistortTexture");
        if (Shader.Find("ErbGameArt/LWRP/Particles/Add_Trail") != null) Add_Trail_LWRP = Shader.Find("ErbGameArt/LWRP/Particles/Add_Trail");
        if (Shader.Find("ErbGameArt/LWRP/Particles/Blend_Electricity") != null) Blend_Electricity_LWRP = Shader.Find("ErbGameArt/LWRP/Particles/Blend_Electricity");
        if (Shader.Find("ErbGameArt/LWRP/Particles/Blend_LitGlow") != null) Blend_LitGlow_LWRP = Shader.Find("ErbGameArt/LWRP/Particles/Blend_LitGlow");
        if (Shader.Find("graphs/LWRP_Lit_CenterGlow") != null) Lit_CenterGlow_LWRP = Shader.Find("graphs/LWRP_Lit_CenterGlow");
        if (Shader.Find("ErbGameArt/LWRP/Particles/Blend_TwoSides") != null) Blend_TwoSides_LWRP = Shader.Find("ErbGameArt/LWRP/Particles/Blend_TwoSides");
        if (Shader.Find("EGA/LWRP/Particles/SmoothSmoke") != null) SmoothSmoke_LWRP = Shader.Find("EGA/LWRP/Particles/SmoothSmoke");
        if (Shader.Find("EGA/LWRP/Particles/Blend_Normals") != null) Blend_Normals_LWRP = Shader.Find("EGA/LWRP/Particles/Blend_Normals");

        if (Shader.Find("EGA/HDRP/Particles/Add_CenterGlow") != null) Add_CenterGlow_HDRP = Shader.Find("EGA/HDRP/Particles/Add_CenterGlow");
        if (Shader.Find("EGA/HDRP/Particles/Blend_CenterGlow") != null) Blend_CenterGlow_HDRP = Shader.Find("EGA/HDRP/Particles/Blend_CenterGlow");
        if (Shader.Find("EGA/HDRP/Particles/Add_DistortTexture") != null) Add_DistortTexture_HDRP = Shader.Find("EGA/HDRP/Particles/Add_DistortTexture");
        if (Shader.Find("EGA/HDRP/Particles/Blend_DistortTexture") != null) Blend_DistortTexture_HDRP = Shader.Find("EGA/HDRP/Particles/Blend_DistortTexture");
        if (Shader.Find("EGA/HDRP/Particles/Add_Trail") != null) Add_Trail_HDRP = Shader.Find("EGA/HDRP/Particles/Add_Trail");
        if (Shader.Find("EGA/HDRP/Particles/Blend_Electricity") != null) Blend_Electricity_HDRP = Shader.Find("EGA/HDRP/Particles/Blend_Electricity");
        if (Shader.Find("EGA/HDRP/Particles/Blend_LitGlow") != null) Blend_LitGlow_HDRP = Shader.Find("EGA/HDRP/Particles/Blend_LitGlow");
        if (Shader.Find("Shader Graphs/HDRP_Lit_CenterGlow") != null) Lit_CenterGlow_HDRP = Shader.Find("Shader Graphs/HDRP_Lit_CenterGlow");
        if (Shader.Find("EGA/HDRP/Particles/Blend_TwoSides") != null) Blend_TwoSides_HDRP = Shader.Find("EGA/HDRP/Particles/Blend_TwoSides");
        if (Shader.Find("EGA/HDRP/Particles/SmoothSmoke") != null) SmoothSmoke_HDRP = Shader.Find("EGA/HDRP/Particles/SmoothSmoke");
        if (Shader.Find("EGA/HDRP/Particles/Blend_Normals") != null) Blend_Normals_HDRP = Shader.Find("EGA/HDRP/Particles/Blend_Normals");

        string[] folderMat = AssetDatabase.FindAssets("t:Material", new[] { "Assets/ErbGameArt" });
        shaderMaterials = new Material[folderMat.Length];

        for (int i = 0; i < folderMat.Length; i++)
        {
            var patch = AssetDatabase.GUIDToAssetPath(folderMat[i]);
            shaderMaterials[i] = (Material)AssetDatabase.LoadAssetAtPath(patch, typeof(Material));
        }
    }

    private void ChangeToLWRP()
    {
     
        foreach (var material in shaderMaterials)
        {
            if (Shader.Find("ErbGameArt/LWRP/Particles/Add_CenterGlow") != null)
            {
                if (material.shader == Add_CenterGlow || material.shader == Add_CenterGlow_HDRP)
                {
                    material.shader = Add_CenterGlow_LWRP;
                }
            }
            /*----------------------------------------------------------------------------------------------------*/
            if (Shader.Find("ErbGameArt/LWRP/Particles/Blend_CenterGlow") != null)
            {
                if (material.shader == Blend_CenterGlow || material.shader == Blend_CenterGlow_HDRP)
                {
                    material.shader = Blend_CenterGlow_LWRP;
                }
            }
            /*----------------------------------------------------------------------------------------------------*/
            if (Shader.Find("ErbGameArt/LWRP/Particles/Add_DistortTexture") != null)
            {
                if (material.shader == Add_DistortTexture || material.shader == Add_DistortTexture_HDRP)
                {
                    material.shader = Add_DistortTexture_LWRP;
                }
            }
            /*----------------------------------------------------------------------------------------------------*/
            if (Shader.Find("ErbGameArt/LWRP/Particles/Blend_DistortTexture") != null)
            {
                if (material.shader == Blend_DistortTexture || material.shader == Blend_DistortTexture_HDRP)
                {
                    material.shader = Blend_DistortTexture_LWRP;
                }
            }
            /*----------------------------------------------------------------------------------------------------*/
            if (Shader.Find("ErbGameArt/LWRP/Particles/Add_Trail") != null)
            {
                if (material.shader == Add_Trail || material.shader == Add_Trail_HDRP)
                {
                    material.shader = Add_Trail_LWRP;
                }
            }
            /*----------------------------------------------------------------------------------------------------*/
            if (Shader.Find("ErbGameArt/LWRP/Particles/Blend_Electricity") != null)
            {
                if (material.shader == Blend_Electricity || material.shader == Blend_Electricity_HDRP)
                {
                    material.shader = Blend_Electricity_LWRP;
                }
            }
            /*----------------------------------------------------------------------------------------------------*/
            if (Shader.Find("ErbGameArt/LWRP/Particles/Blend_LitGlow") != null)
            {
                if (material.shader == Blend_LitGlow || material.shader == Blend_LitGlow_HDRP)
                {
                    material.shader = Blend_LitGlow_LWRP;
                }
            }
            /*----------------------------------------------------------------------------------------------------*/
            if (Shader.Find("graphs/LWRP_Lit_CenterGlow") != null)
            {
                if (material.shader == Lit_CenterGlow || material.shader == Lit_CenterGlow_HDRP)
                {
                    material.shader = Lit_CenterGlow_LWRP;
                }
            }
            /*----------------------------------------------------------------------------------------------------*/
            if (Shader.Find("ErbGameArt/LWRP/Particles/Blend_TwoSides") != null)
            {
                if (material.shader == Blend_TwoSides || material.shader == Blend_TwoSides_HDRP)
                {
                    material.shader = Blend_TwoSides_LWRP;
                }
            }
            /*----------------------------------------------------------------------------------------------------*/
            if (Shader.Find("EGA/LWRP/Particles/SmoothSmoke") != null)
            {
                if (material.shader == SmoothSmoke || material.shader == SmoothSmoke_HDRP)
                {
                    material.shader = SmoothSmoke_LWRP;
                }
            }
            /*----------------------------------------------------------------------------------------------------*/
            if (Shader.Find("EGA/LWRP/Particles/Blend_Normals") != null)
            {
                if (material.shader == Blend_Normals || material.shader == Blend_Normals_HDRP)
                {
                    material.shader = Blend_Normals_LWRP;
                }
            }
        }
    }


    private void ChangeToSRP()
    {

        foreach (var material in shaderMaterials)
        {
            if (Shader.Find("EGA/Particles/Add_CenterGlow") != null)
            {
                if (material.shader == Add_CenterGlow_LWRP || material.shader == Add_CenterGlow_HDRP)
                {
                    material.shader = Add_CenterGlow;
                }
            }
            /*----------------------------------------------------------------------------------------------------*/
            if (Shader.Find("EGA/Particles/Blend_CenterGlow") != null)
            {
                if (material.shader == Blend_CenterGlow_LWRP || material.shader == Blend_CenterGlow_HDRP)
                {
                    material.shader = Blend_CenterGlow;
                }
            }
            /*----------------------------------------------------------------------------------------------------*/
            if (Shader.Find("EGA/Particles/Add_DistortTexture") != null)
            {
                if (material.shader == Add_DistortTexture_LWRP || material.shader == Add_DistortTexture_HDRP)
                {
                    material.shader = Add_DistortTexture;
                }
            }
            /*----------------------------------------------------------------------------------------------------*/
            if (Shader.Find("EGA/Particles/Blend_DistortTexture") != null)
            {
                if (material.shader == Blend_DistortTexture_LWRP || material.shader == Blend_DistortTexture_HDRP)
                {
                    material.shader = Blend_DistortTexture;
                }
            }
            /*----------------------------------------------------------------------------------------------------*/
            if (Shader.Find("EGA/Particles/Add_Trail") != null)
            {
                if (material.shader == Add_Trail_LWRP || material.shader == Add_Trail_HDRP)
                {
                    material.shader = Add_Trail;
                }
            }
            /*----------------------------------------------------------------------------------------------------*/
            if (Shader.Find("EGA/Particles/Blend_Electricity") != null)
            {
                if (material.shader == Blend_Electricity_LWRP || material.shader == Blend_Electricity_HDRP)
                {
                    material.shader = Blend_Electricity;
                }
            }
            /*----------------------------------------------------------------------------------------------------*/
            if (Shader.Find("EGA/Particles/Blend_LitGlow") != null)
            {
                if (material.shader == Blend_LitGlow_LWRP || material.shader == Blend_LitGlow_HDRP)
                {
                    material.shader = Blend_LitGlow;
                }
            }
            /*----------------------------------------------------------------------------------------------------*/
            if (Shader.Find("EGA/Particles/Lit_CenterGlow") != null)
            {
                if (material.shader == Lit_CenterGlow_LWRP || material.shader == Lit_CenterGlow_HDRP)
                {
                    material.shader = Lit_CenterGlow;
                }
            }
            /*----------------------------------------------------------------------------------------------------*/
            if (Shader.Find("EGA/Particles/Blend_TwoSides") != null)
            {
                if (material.shader == Blend_TwoSides_LWRP || material.shader == Blend_TwoSides_HDRP)
                {
                    material.shader = Blend_TwoSides;
                }
            }
            /*----------------------------------------------------------------------------------------------------*/
            if (Shader.Find("EGA/Particles/SmoothSmoke") != null)
            {
                if (material.shader == SmoothSmoke_LWRP || material.shader == SmoothSmoke_HDRP)
                {
                    material.shader = SmoothSmoke;
                }
            }
            /*----------------------------------------------------------------------------------------------------*/
            if (Shader.Find("EGA/Particles/Blend_Normals") != null)
            {
                if (material.shader == Blend_Normals_LWRP || material.shader == Blend_Normals_HDRP)
                {
                    material.shader = Blend_Normals;
                }
            }
        }
    }

    private void ChangeToHDRP()
    {
        foreach (var material in shaderMaterials)
        {
            if (Shader.Find("EGA/HDRP/Particles/Add_CenterGlow") != null)
            {
                if (material.shader == Add_CenterGlow || material.shader == Add_CenterGlow_LWRP)
                {
                    material.shader = Add_CenterGlow_HDRP;
                }
            }
            /*----------------------------------------------------------------------------------------------------*/
            if (Shader.Find("EGA/HDRP/Particles/Blend_CenterGlow") != null)
            {
                if (material.shader == Blend_CenterGlow || material.shader == Blend_CenterGlow_LWRP)
                {
                    material.shader = Blend_CenterGlow_HDRP;
                }
            }
            /*----------------------------------------------------------------------------------------------------*/
            if (Shader.Find("EGA/HDRP/Particles/Add_DistortTexture") != null)
            {
                if (material.shader == Add_DistortTexture || material.shader == Add_DistortTexture_LWRP)
                {
                    material.shader = Add_DistortTexture_HDRP;
                }
            }
            /*----------------------------------------------------------------------------------------------------*/
            if (Shader.Find("EGA/HDRP/Particles/Blend_DistortTexture") != null)
            {
                if (material.shader == Blend_DistortTexture || material.shader == Blend_DistortTexture_LWRP)
                {
                    material.shader = Blend_DistortTexture_HDRP;
                }
            }
            /*----------------------------------------------------------------------------------------------------*/
            if (Shader.Find("EGA/HDRP/Particles/Add_Trail") != null)
            {
                if (material.shader == Add_Trail || material.shader == Add_Trail_LWRP)
                {
                    material.shader = Add_Trail_HDRP;
                }
            }
            /*----------------------------------------------------------------------------------------------------*/
            if (Shader.Find("EGA/HDRP/Particles/Blend_Electricity") != null)
            {
                if (material.shader == Blend_Electricity || material.shader == Blend_Electricity_LWRP)
                {
                    material.shader = Blend_Electricity_HDRP;
                }
            }
            /*----------------------------------------------------------------------------------------------------*/
            if (Shader.Find("EGA/HDRP/Particles/Blend_LitGlow") != null)
            {
                if (material.shader == Blend_LitGlow || material.shader == Blend_LitGlow_LWRP)
                {
                    material.shader = Blend_LitGlow_HDRP;
                }
            }
            /*----------------------------------------------------------------------------------------------------*/
            if (Shader.Find("Shader Graphs/HDRP_Lit_CenterGlow") != null)
            {
                if (material.shader == Lit_CenterGlow || material.shader == Lit_CenterGlow_LWRP)
                {
                    material.shader = Lit_CenterGlow_HDRP;
                }
            }
            /*----------------------------------------------------------------------------------------------------*/
            if (Shader.Find("EGA/HDRP/Particles/Blend_TwoSides") != null)
            {
                if (material.shader == Blend_TwoSides || material.shader == Blend_TwoSides_LWRP)
                {
                    material.shader = Blend_TwoSides_HDRP;
                }
            }
            /*----------------------------------------------------------------------------------------------------*/
            if (Shader.Find("EGA/HDRP/Particles/SmoothSmoke") != null)
            {
                if (material.shader == SmoothSmoke || material.shader == SmoothSmoke_LWRP)
                {
                    material.shader = SmoothSmoke_HDRP;
                }
            }
            /*----------------------------------------------------------------------------------------------------*/
            if (Shader.Find("EGA/HDRP/Particles/Blend_Normals") != null)
            {
                if (material.shader == Blend_Normals || material.shader == Blend_Normals_LWRP)
                {
                    material.shader = Blend_Normals_HDRP;
                }
            }
        }
    }
}