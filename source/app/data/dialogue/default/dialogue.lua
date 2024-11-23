return {
  metadata = {},
  nodes = {
    ["6b10a59c-5300-4ec3-aa8c-fe59ebedce61"] = {
      branches = {
        {
          op = 1,
          value = true,
          variable = "Fallback.FallbackState"
        },
        {
          op = 1,
          value = true,
          variable = "Fallback.FallbackState"
        }
      },
      children = {
        "6292305f-9cd9-4a6d-a388-cd8d6691bcac",
        "bf3c469e-b2b9-4ee4-9b56-884cdb26f8d9"
      },
      combinator = 1,
      description = "",
      is_entry_point = false,
      kind = "Branch",
      uuid = "6b10a59c-5300-4ec3-aa8c-fe59ebedce61"
    },
    ["9a3b3dfe-dd3f-47ed-a393-273a57648d2d"] = {
      children = {
        "6b10a59c-5300-4ec3-aa8c-fe59ebedce61"
      },
      is_entry_point = false,
      kind = "Continue",
      mute = false,
      uuid = "9a3b3dfe-dd3f-47ed-a393-273a57648d2d"
    },
    ["944fef5d-ef65-40d4-8722-5c19d4316678"] = {
      children = {
        "a32c09e5-ffd4-4ab5-903b-d48571b1422a"
      },
      export = false,
      is_entry_point = false,
      kind = "Label",
      label = "Again",
      uuid = "944fef5d-ef65-40d4-8722-5c19d4316678"
    },
    ["6292305f-9cd9-4a6d-a388-cd8d6691bcac"] = {
      children = {
        "b2a52f13-e59d-4973-ac25-670f87b75970"
      },
      color = {
        a = 1,
        b = 1,
        g = 1,
        r = 1
      },
      color_id = "",
      is_entry_point = false,
      kind = "Text",
      text = "The branch was false.",
      uuid = "6292305f-9cd9-4a6d-a388-cd8d6691bcac",
      who = ""
    },
    ["a32c09e5-ffd4-4ab5-903b-d48571b1422a"] = {
      children = {
        "9a3b3dfe-dd3f-47ed-a393-273a57648d2d"
      },
      color = {
        a = 1,
        b = 1,
        g = 1,
        r = 1
      },
      color_id = "",
      is_entry_point = true,
      kind = "Text",
      text = "This is the entry point.",
      uuid = "a32c09e5-ffd4-4ab5-903b-d48571b1422a",
      who = ""
    },
    ["b2a52f13-e59d-4973-ac25-670f87b75970"] = {
      children = {},
      is_entry_point = false,
      kind = "Jump",
      target = "Again",
      target_dialogue = "",
      uuid = "b2a52f13-e59d-4973-ac25-670f87b75970"
    },
    ["bf3c469e-b2b9-4ee4-9b56-884cdb26f8d9"] = {
      children = {
        "cb435763-595b-4e44-8602-6600a9a20d89"
      },
      color = {
        a = 1,
        b = 1,
        g = 1,
        r = 1
      },
      color_id = "",
      is_entry_point = false,
      kind = "Text",
      text = "The branch was true. Here is some more text, to see how it all works.",
      uuid = "bf3c469e-b2b9-4ee4-9b56-884cdb26f8d9",
      who = ""
    },
    ["cb435763-595b-4e44-8602-6600a9a20d89"] = {
      children = {},
      is_entry_point = false,
      kind = "Jump",
      target = "Again",
      target_dialogue = "",
      uuid = "cb435763-595b-4e44-8602-6600a9a20d89"
    }
  }
}