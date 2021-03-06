#pragma once

#include "CheatChicken/Scripts.h"

#define DIALOGUE_STARTING_MOOD 4
#define DIALOGUE_DEFAULT_IQ_TEST 4

variable __dialogue_currently_open;
variable __dialogue_current_reply_name;
variable __dialogue_current_option_name_list;

procedure dialogue_start(variable scenario);

procedure __dialogue_end begin
    // Nothing, end dialogue
end

procedure __dialogue_option_one begin
    variable option_name = __dialogue_current_option_name_list[0];

    if map_contains_key(config.Actions, option_name) then begin
        variable action_list = string_split(config.Actions[option_name], ",");
        variable action;
        foreach action in action_list begin
            if string_starts_with(action, "D:") then begin
                variable dialogue_scenario = substr(action, 2, 0); // 2 is "D:"
                call dialogue_start(dialogue_scenario);
            end else
                SignalNamed(action);
        end
    end
end

procedure __dialogue_option_two begin
    display1("option:", __dialogue_current_option_name_list[1]);
end

procedure __dialogue_option_three begin
    display1("option:", __dialogue_current_option_name_list[2]);
end

procedure __dialogue_option_four begin
    display1("option:", __dialogue_current_option_name_list[3]);
end

procedure __dialogue_option_five begin
    display1("option:", __dialogue_current_option_name_list[4]);
end

procedure __dialogue_option_six begin
    display1("option:", __dialogue_current_option_name_list[5]);
end

procedure __dialogue_begin begin
    variable reply_id = atoi(config.Messages[__dialogue_current_reply_name]);
    Reply(reply_id);

    if len_array(__dialogue_current_option_name_list) > 0 then
        NOption(atoi(config.Messages[__dialogue_current_option_name_list[0]]), __dialogue_option_one, DIALOGUE_DEFAULT_IQ_TEST);
    if len_array(__dialogue_current_option_name_list) > 1 then
        NOption(atoi(config.Messages[__dialogue_current_option_name_list[1]]), __dialogue_option_two, DIALOGUE_DEFAULT_IQ_TEST);
    if len_array(__dialogue_current_option_name_list) > 2 then
        NOption(atoi(config.Messages[__dialogue_current_option_name_list[2]]), __dialogue_option_three, DIALOGUE_DEFAULT_IQ_TEST);
    if len_array(__dialogue_current_option_name_list) > 3 then
        NOption(atoi(config.Messages[__dialogue_current_option_name_list[3]]), __dialogue_option_four, DIALOGUE_DEFAULT_IQ_TEST);
    if len_array(__dialogue_current_option_name_list) > 4 then
        NOption(atoi(config.Messages[__dialogue_current_option_name_list[4]]), __dialogue_option_five, DIALOGUE_DEFAULT_IQ_TEST);
    if len_array(__dialogue_current_option_name_list) > 5 then
        NOption(atoi(config.Messages[__dialogue_current_option_name_list[5]]), __dialogue_option_six, DIALOGUE_DEFAULT_IQ_TEST);
end

procedure dialogue_start(variable scenario) begin
    variable scenario_reply_and_options = string_split(config.Dialogue[scenario], ",");

    // The first in the list is the reply message name
    __dialogue_current_reply_name = scenario_reply_and_options[0];

    // Remove the reply message from the array
    call array_cut(scenario_reply_and_options, 0, 1);

    // The names of the options for this dialogue scnario
    __dialogue_current_option_name_list = scenario_reply_and_options;
    fix_array(__dialogue_current_option_name_list);

    variable dialogue_script_id = get_script_id(config.Dialogue_Config.script);
    display1("Dialogue script ID:", dialogue_script_id);

    if __dialogue_currently_open then
        call __dialogue_begin;
    else begin
        __dialogue_currently_open = true;
        start_gdialog(dialogue_script_id, self_obj, DIALOGUE_STARTING_MOOD, -1, -1);
        gSay_Start;
        call __dialogue_begin;
        gSay_End;
        end_dialogue;
        __dialogue_currently_open = false;
    end
end