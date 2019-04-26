//
//  imgui-user.h
//  t02
//
//  Created by Daniel Harvey on 13/04/2019.
//  Copyright © 2019 Cordry, Julien. All rights reserved.
//

#ifndef imgui_user_h
#define imgui_user_h

#include <stdio.h>
#include <string>
#include <vector>
//https://eliasdaler.github.io/using-imgui-with-sfml-pt2/#using-imgui-with-stl
namespace ImGui
{
    static auto vector_getter = [](void* vec, int idx, const char** out_text)
    {
        auto& vector = *static_cast<std::vector<std::string>*>(vec);
        if (idx < 0 || idx >= static_cast<int>(vector.size()))
        {
            return false;
            
        }
        *out_text = vector.at(idx).c_str();
        return true;
    };
    
    IMGUI_API inline bool Combo(const char* label, int* currIndex, std::vector<std::string>& values)
    {
        if (values.empty())
        {
            return false;
        }
        return Combo(label, currIndex, vector_getter, static_cast<void*>(&values), (int)values.size());
    }
    
    IMGUI_API inline bool ListBox(const char* label, int* currIndex, std::vector<std::string>& values)
    {
        if (values.empty())
        {
            return false;
        }
        return ListBox(label, currIndex, vector_getter,static_cast<void*>(&values), (int)values.size());
    }
    
}

#endif /* imgui_wrapper_hpp */
