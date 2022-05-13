/*************************************************************************/
/*  script_language_extension.h                                          */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                      https://godotengine.org                          */
/*************************************************************************/
/* Copyright (c) 2007-2022 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2022 Godot Engine contributors (cf. AUTHORS.md).   */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#ifndef SCRIPT_LANGUAGE_EXTENSION_H
#define SCRIPT_LANGUAGE_EXTENSION_H

#include "core/extension/ext_wrappers.gen.inc"
#include "core/object/gdvirtual.gen.inc"
#include "core/object/script_language.h"
#include "core/variant/native_ptr.h"
#include "core/variant/typed_array.h"

class ScriptExtension : public Script {
	GDCLASS(ScriptExtension, Script)

protected:
	EXBIND0R(bool, editor_can_reload_from_file)

	GDVIRTUAL1(_placeholder_erased, GDNativePtr<void>)
	virtual void _placeholder_erased(PlaceHolderScriptInstance *p_placeholder) override {
		GDVIRTUAL_CALL(_placeholder_erased, p_placeholder);
	}

	static void _bind_methods();

public:
	EXBIND0RC(bool, can_instantiate)
	EXBIND0RC(Ref<Script>, get_base_script)
	EXBIND1RC(bool, inherits_script, const Ref<Script> &)
	EXBIND0RC(StringName, get_instance_base_type)

	GDVIRTUAL1RC(GDNativePtr<void>, _instance_create, Object *)
	virtual ScriptInstance *instance_create(Object *p_this) override {
		GDNativePtr<void> ret = nullptr;
		GDVIRTUAL_REQUIRED_CALL(_instance_create, p_this, ret);
		return reinterpret_cast<ScriptInstance *>(ret.operator void *());
	}
	GDVIRTUAL1RC(GDNativePtr<void>, _placeholder_instance_create, Object *)
	PlaceHolderScriptInstance *placeholder_instance_create(Object *p_this) override {
		GDNativePtr<void> ret = nullptr;
		GDVIRTUAL_REQUIRED_CALL(_placeholder_instance_create, p_this, ret);
		return reinterpret_cast<PlaceHolderScriptInstance *>(ret.operator void *());
	}

	EXBIND1RC(bool, instance_has, const Object *)
	EXBIND0RC(bool, has_source_code)
	EXBIND0RC(String, get_source_code)
	EXBIND1(set_source_code, const String &)
	EXBIND1R(Error, reload, bool)

	GDVIRTUAL0RC(TypedArray<Dictionary>, _get_documentation)
#ifdef TOOLS_ENABLED
	virtual Vector<DocData::ClassDoc> get_documentation() const override {
		TypedArray<Dictionary> doc;
		GDVIRTUAL_REQUIRED_CALL(_get_documentation, doc);

		Vector<DocData::ClassDoc> class_doc;
#ifndef _MSC_VER
#warning missing conversion from documentation to ClassDoc
#endif
		return class_doc;
	}
#endif // TOOLS_ENABLED

	EXBIND1RC(bool, has_method, const StringName &)

	GDVIRTUAL1RC(Dictionary, _get_method_info, const StringName &)
	virtual MethodInfo get_method_info(const StringName &p_method) const override {
		Dictionary mi;
		GDVIRTUAL_REQUIRED_CALL(_get_method_info, p_method, mi);
		return MethodInfo::from_dict(mi);
	}

	EXBIND0RC(bool, is_tool)
	EXBIND0RC(bool, is_valid)

	EXBIND0RC(ScriptLanguage *, get_language)
	EXBIND1RC(bool, has_script_signal, const StringName &)

	GDVIRTUAL0RC(TypedArray<Dictionary>, _get_script_signal_list)

	virtual void get_script_signal_list(List<MethodInfo> *r_signals) const override {
		TypedArray<Dictionary> sl;
		GDVIRTUAL_REQUIRED_CALL(_get_script_signal_list, sl);
		for (int i = 0; i < sl.size(); i++) {
			r_signals->push_back(MethodInfo::from_dict(sl[i]));
		}
	}

	GDVIRTUAL1RC(bool, _has_property_default_value, const StringName &)
	GDVIRTUAL1RC(Variant, _get_property_default_value, const StringName &)

	virtual bool get_property_default_value(const StringName &p_property, Variant &r_value) const override {
		bool has_dv = false;
		if (!GDVIRTUAL_REQUIRED_CALL(_has_property_default_value, p_property, has_dv) || !has_dv) {
			return false;
		}
		Variant ret;
		GDVIRTUAL_REQUIRED_CALL(_get_property_default_value, p_property, ret);
		return ret;
	}

	EXBIND0(update_exports)

	GDVIRTUAL0RC(TypedArray<Dictionary>, _get_script_method_list)

	virtual void get_script_method_list(List<MethodInfo> *r_methods) const override {
		TypedArray<Dictionary> sl;
		GDVIRTUAL_REQUIRED_CALL(_get_script_method_list, sl);
		for (int i = 0; i < sl.size(); i++) {
			r_methods->push_back(MethodInfo::from_dict(sl[i]));
		}
	}

	GDVIRTUAL0RC(TypedArray<Dictionary>, _get_script_property_list)

	virtual void get_script_property_list(List<PropertyInfo> *r_propertys) const override {
		TypedArray<Dictionary> sl;
		GDVIRTUAL_REQUIRED_CALL(_get_script_property_list, sl);
		for (int i = 0; i < sl.size(); i++) {
			r_propertys->push_back(PropertyInfo::from_dict(sl[i]));
		}
	}

	EXBIND1RC(int, get_member_line, const StringName &)

	GDVIRTUAL0RC(Dictionary, _get_constants)

	virtual void get_constants(HashMap<StringName, Variant> *p_constants) override {
		Dictionary constants;
		GDVIRTUAL_REQUIRED_CALL(_get_constants, constants);
		List<Variant> keys;
		constants.get_key_list(&keys);
		for (const Variant &K : keys) {
			p_constants->insert(K, constants[K]);
		}
	}
	GDVIRTUAL0RC(TypedArray<StringName>, _get_members)
	virtual void get_members(RBSet<StringName> *p_members) override {
		TypedArray<StringName> members;
		GDVIRTUAL_REQUIRED_CALL(_get_members, members);
		for (int i = 0; i < members.size(); i++) {
			p_members->insert(members[i]);
		}
	}

	EXBIND0RC(bool, is_placeholder_fallback_enabled)

	GDVIRTUAL0RC(TypedArray<Dictionary>, _get_rpc_methods)

	virtual const Vector<Multiplayer::RPCConfig> get_rpc_methods() const override {
		TypedArray<Dictionary> ret;
		GDVIRTUAL_REQUIRED_CALL(_get_rpc_methods, ret);
		Vector<Multiplayer::RPCConfig> rpcret;
		for (int i = 0; i < ret.size(); i++) {
			Dictionary d = ret[i];
			Multiplayer::RPCConfig rpc;
			ERR_CONTINUE(!d.has("name"));
			rpc.name = d["name"];
			ERR_CONTINUE(!d.has("rpc_mode"));
			rpc.rpc_mode = Multiplayer::RPCMode(int(d["rpc_mode"]));
			ERR_CONTINUE(!d.has("call_local"));
			rpc.call_local = d["call_local"];
			ERR_CONTINUE(!d.has("transfer_mode"));
			rpc.transfer_mode = Multiplayer::TransferMode(int(d["transfer_mode"]));
			ERR_CONTINUE(!d.has("channel"));
			rpc.channel = d["channel"];
			rpcret.push_back(rpc);
		}
		return rpcret;
	}

	ScriptExtension() {}
};

typedef ScriptLanguage::ProfilingInfo ScriptLanguageExtensionProfilingInfo;

GDVIRTUAL_NATIVE_PTR(ScriptLanguageExtensionProfilingInfo)

class ScriptLanguageExtension : public ScriptLanguage {
	GDCLASS(ScriptLanguageExtension, ScriptLanguage)
protected:
	static void _bind_methods();

public:
	EXBIND0RC(String, get_name)

	EXBIND0(init)
	EXBIND0RC(String, get_type)
	EXBIND0RC(String, get_extension)
	EXBIND1R(Error, execute_file, const String &)
	EXBIND0(finish)

	/* EDITOR FUNCTIONS */

	GDVIRTUAL0RC(Vector<String>, _get_reserved_words)

	virtual void get_reserved_words(List<String> *p_words) const override {
		Vector<String> ret;
		GDVIRTUAL_REQUIRED_CALL(_get_reserved_words, ret);
		for (int i = 0; i < ret.size(); i++) {
			p_words->push_back(ret[i]);
		}
	}
	EXBIND1RC(bool, is_control_flow_keyword, String)

	GDVIRTUAL0RC(Vector<String>, _get_comment_delimiters)

	virtual void get_comment_delimiters(List<String> *p_words) const override {
		Vector<String> ret;
		GDVIRTUAL_REQUIRED_CALL(_get_comment_delimiters, ret);
		for (int i = 0; i < ret.size(); i++) {
			p_words->push_back(ret[i]);
		}
	}

	GDVIRTUAL0RC(Vector<String>, _get_string_delimiters)

	virtual void get_string_delimiters(List<String> *p_words) const override {
		Vector<String> ret;
		GDVIRTUAL_REQUIRED_CALL(_get_string_delimiters, ret);
		for (int i = 0; i < ret.size(); i++) {
			p_words->push_back(ret[i]);
		}
	}

	EXBIND3RC(Ref<Script>, make_template, const String &, const String &, const String &)

	GDVIRTUAL1RC(TypedArray<Dictionary>, _get_built_in_templates, StringName)

	virtual Vector<ScriptTemplate> get_built_in_templates(StringName p_object) override {
		TypedArray<Dictionary> ret;
		GDVIRTUAL_REQUIRED_CALL(_get_built_in_templates, p_object, ret);
		Vector<ScriptTemplate> stret;
		for (int i = 0; i < ret.size(); i++) {
			Dictionary d = ret[i];
			ScriptTemplate st;
			ERR_CONTINUE(!d.has("inherit"));
			st.inherit = d["inherit"];
			ERR_CONTINUE(!d.has("name"));
			st.name = d["name"];
			ERR_CONTINUE(!d.has("description"));
			st.description = d["description"];
			ERR_CONTINUE(!d.has("content"));
			st.content = d["content"];
			ERR_CONTINUE(!d.has("id"));
			st.id = d["id"];
			ERR_CONTINUE(!d.has("origin"));
			st.origin = TemplateLocation(int(d["origin"]));
			stret.push_back(st);
		}
		return stret;
	}

	EXBIND0R(bool, is_using_templates)

	GDVIRTUAL6RC(Dictionary, _validate, const String &, const String &, bool, bool, bool, bool)
	virtual bool validate(const String &p_script, const String &p_path = "", List<String> *r_functions = nullptr, List<ScriptError> *r_errors = nullptr, List<Warning> *r_warnings = nullptr, RBSet<int> *r_safe_lines = nullptr) const override {
		Dictionary ret;
		GDVIRTUAL_REQUIRED_CALL(_validate, p_script, p_path, r_functions != nullptr, r_errors != nullptr, r_warnings != nullptr, r_safe_lines != nullptr, ret);
		if (!ret.has("valid")) {
			return false;
		}
		if (r_functions != nullptr && ret.has("functions")) {
			Vector<String> functions = ret["functions"];
			for (int i = 0; i < functions.size(); i++) {
				r_functions->push_back(functions[i]);
			}
		}
		if (r_errors != nullptr && ret.has("errors")) {
			Array errors = ret["errors"];
			for (int i = 0; i < errors.size(); i++) {
				Dictionary err = errors[i];
				ERR_CONTINUE(!err.has("line"));
				ERR_CONTINUE(!err.has("column"));
				ERR_CONTINUE(!err.has("message"));

				ScriptError serr;
				serr.line = err["line"];
				serr.column = err["column"];
				serr.message = err["message"];

				r_errors->push_back(serr);
			}
		}
		if (r_warnings != nullptr && ret.has("warnings")) {
			ERR_FAIL_COND_V(!ret.has("warnings"), false);
			Array warnings = ret["warnings"];
			for (int i = 0; i < warnings.size(); i++) {
				Dictionary warn = warnings[i];
				ERR_CONTINUE(!warn.has("start_line"));
				ERR_CONTINUE(!warn.has("end_line"));
				ERR_CONTINUE(!warn.has("leftmost_column"));
				ERR_CONTINUE(!warn.has("rightmost_column"));
				ERR_CONTINUE(!warn.has("code"));
				ERR_CONTINUE(!warn.has("string_code"));
				ERR_CONTINUE(!warn.has("message"));

				Warning swarn;
				swarn.start_line = warn["start_line"];
				swarn.end_line = warn["end_line"];
				swarn.leftmost_column = warn["leftmost_column"];
				swarn.rightmost_column = warn["rightmost_column"];
				swarn.code = warn["code"];
				swarn.string_code = warn["string_code"];
				swarn.message = warn["message"];

				r_warnings->push_back(swarn);
			}
		}
		if (r_safe_lines != nullptr && ret.has("safe_lines")) {
			PackedInt32Array safe_lines = ret["safe_lines"];
			for (int i = 0; i < safe_lines.size(); i++) {
				r_safe_lines->insert(safe_lines[i]);
			}
		}
		return ret["valid"];
	}

	EXBIND1RC(String, validate_path, const String &)
	GDVIRTUAL0RC(Object *, _create_script)
	Script *create_script() const override {
		Object *ret = nullptr;
		GDVIRTUAL_REQUIRED_CALL(_create_script, ret);
		return Object::cast_to<Script>(ret);
	}
	EXBIND0RC(bool, has_named_classes)
	EXBIND0RC(bool, supports_builtin_mode)
	EXBIND0RC(bool, supports_documentation)
	EXBIND0RC(bool, can_inherit_from_file)

	EXBIND2RC(int, find_function, const String &, const String &)
	EXBIND3RC(String, make_function, const String &, const String &, const PackedStringArray &)
	EXBIND3R(Error, open_in_external_editor, const Ref<Script> &, int, int)
	EXBIND0R(bool, overrides_external_editor)

	GDVIRTUAL3RC(Dictionary, _complete_code, const String &, const String &, Object *)

	virtual Error complete_code(const String &p_code, const String &p_path, Object *p_owner, List<CodeCompletionOption> *r_options, bool &r_force, String &r_call_hint) override {
		Dictionary ret;
		GDVIRTUAL_REQUIRED_CALL(_complete_code, p_code, p_path, p_owner, ret);
		if (!ret.has("result")) {
			return ERR_UNAVAILABLE;
		}

		if (r_options != nullptr && ret.has("options")) {
			Array options = ret["options"];
			for (int i = 0; i < options.size(); i++) {
				Dictionary op = options[i];
				CodeCompletionOption option;
				ERR_CONTINUE(!op.has("kind"));
				option.kind = CodeCompletionKind(int(op["kind"]));
				ERR_CONTINUE(!op.has("display"));
				option.display = op["display"];
				ERR_CONTINUE(!op.has("insert_text"));
				option.insert_text = op["insert_text"];
				ERR_CONTINUE(!op.has("font_color"));
				option.font_color = op["font_color"];
				ERR_CONTINUE(!op.has("icon"));
				option.icon = op["icon"];
				ERR_CONTINUE(!op.has("default_value"));
				option.default_value = op["default_value"];
				ERR_CONTINUE(!op.has("location"));
				option.location = op["location"];
				if (op.has("matches")) {
					PackedInt32Array matches = op["matches"];
					ERR_CONTINUE(matches.size() & 1);
					for (int j = 0; j < matches.size(); j += 2) {
						option.matches.push_back(Pair<int, int>(matches[j], matches[j + 1]));
					}
				}
				r_options->push_back(option);
			}
		}

		ERR_FAIL_COND_V(!ret.has("force"), ERR_UNAVAILABLE);
		r_force = ret["force"];
		ERR_FAIL_COND_V(!ret.has("call_hint"), ERR_UNAVAILABLE);
		r_call_hint = ret["call_hint"];
		ERR_FAIL_COND_V(!ret.has("result"), ERR_UNAVAILABLE);
		Error result = Error(int(ret["result"]));

		return result;
	}

	GDVIRTUAL4RC(Dictionary, _lookup_code, const String &, const String &, const String &, Object *)

	virtual Error lookup_code(const String &p_code, const String &p_symbol, const String &p_path, Object *p_owner, LookupResult &r_result) override {
		Dictionary ret;
		GDVIRTUAL_REQUIRED_CALL(_lookup_code, p_code, p_symbol, p_path, p_owner, ret);
		if (!ret.has("result")) {
			return ERR_UNAVAILABLE;
		}

		ERR_FAIL_COND_V(!ret.has("type"), ERR_UNAVAILABLE);
		r_result.type = LookupResultType(int(ret["type"]));
		ERR_FAIL_COND_V(!ret.has("script"), ERR_UNAVAILABLE);
		r_result.script = ret["script"];
		ERR_FAIL_COND_V(!ret.has("class_name"), ERR_UNAVAILABLE);
		r_result.class_name = ret["class_name"];
		ERR_FAIL_COND_V(!ret.has("class_path"), ERR_UNAVAILABLE);
		r_result.class_path = ret["class_path"];
		ERR_FAIL_COND_V(!ret.has("location"), ERR_UNAVAILABLE);
		r_result.location = ret["location"];

		Error result = Error(int(ret["result"]));

		return result;
	}

	GDVIRTUAL3RC(String, _auto_indent_code, const String &, int, int)
	virtual void auto_indent_code(String &p_code, int p_from_line, int p_to_line) const override {
		String ret;
		GDVIRTUAL_REQUIRED_CALL(_auto_indent_code, p_code, p_from_line, p_to_line, ret);
		p_code = ret;
	}
	EXBIND2(add_global_constant, const StringName &, const Variant &)
	EXBIND2(add_named_global_constant, const StringName &, const Variant &)
	EXBIND1(remove_named_global_constant, const StringName &)

	/* MULTITHREAD FUNCTIONS */

	//some VMs need to be notified of thread creation/exiting to allocate a stack
	EXBIND0(thread_enter)
	EXBIND0(thread_exit)

	EXBIND0RC(String, debug_get_error)
	EXBIND0RC(int, debug_get_stack_level_count)
	EXBIND1RC(int, debug_get_stack_level_line, int)
	EXBIND1RC(String, debug_get_stack_level_function, int)
	EXBIND1RC(String, debug_get_stack_level_source, int)

	GDVIRTUAL3R(Dictionary, _debug_get_stack_level_locals, int, int, int)
	virtual void debug_get_stack_level_locals(int p_level, List<String> *p_locals, List<Variant> *p_values, int p_max_subitems = -1, int p_max_depth = -1) override {
		Dictionary ret;
		GDVIRTUAL_REQUIRED_CALL(_debug_get_stack_level_locals, p_level, p_max_subitems, p_max_depth, ret);
		if (ret.size() == 0) {
			return;
		}
		if (p_locals != nullptr && ret.has("locals")) {
			PackedStringArray strings = ret["locals"];
			for (int i = 0; i < strings.size(); i++) {
				p_locals->push_back(strings[i]);
			}
		}
		if (p_values != nullptr && ret.has("values")) {
			Array values = ret["values"];
			for (int i = 0; i < values.size(); i++) {
				p_values->push_back(values[i]);
			}
		}
	}
	GDVIRTUAL3R(Dictionary, _debug_get_stack_level_members, int, int, int)
	virtual void debug_get_stack_level_members(int p_level, List<String> *p_members, List<Variant> *p_values, int p_max_subitems = -1, int p_max_depth = -1) override {
		Dictionary ret;
		GDVIRTUAL_REQUIRED_CALL(_debug_get_stack_level_members, p_level, p_max_subitems, p_max_depth, ret);
		if (ret.size() == 0) {
			return;
		}
		if (p_members != nullptr && ret.has("members")) {
			PackedStringArray strings = ret["members"];
			for (int i = 0; i < strings.size(); i++) {
				p_members->push_back(strings[i]);
			}
		}
		if (p_values != nullptr && ret.has("values")) {
			Array values = ret["values"];
			for (int i = 0; i < values.size(); i++) {
				p_values->push_back(values[i]);
			}
		}
	}
	GDVIRTUAL1R(GDNativePtr<void>, _debug_get_stack_level_instance, int)

	virtual ScriptInstance *debug_get_stack_level_instance(int p_level) override {
		GDNativePtr<void> ret = nullptr;
		GDVIRTUAL_REQUIRED_CALL(_debug_get_stack_level_instance, p_level, ret);
		return reinterpret_cast<ScriptInstance *>(ret.operator void *());
	}
	GDVIRTUAL2R(Dictionary, _debug_get_globals, int, int)
	virtual void debug_get_globals(List<String> *p_globals, List<Variant> *p_values, int p_max_subitems = -1, int p_max_depth = -1) override {
		Dictionary ret;
		GDVIRTUAL_REQUIRED_CALL(_debug_get_globals, p_max_subitems, p_max_depth, ret);
		if (ret.size() == 0) {
			return;
		}
		if (p_globals != nullptr && ret.has("globals")) {
			PackedStringArray strings = ret["globals"];
			for (int i = 0; i < strings.size(); i++) {
				p_globals->push_back(strings[i]);
			}
		}
		if (p_values != nullptr && ret.has("values")) {
			Array values = ret["values"];
			for (int i = 0; i < values.size(); i++) {
				p_values->push_back(values[i]);
			}
		}
	}

	EXBIND4R(String, debug_parse_stack_level_expression, int, const String &, int, int)

	GDVIRTUAL0R(TypedArray<Dictionary>, _debug_get_current_stack_info)
	virtual Vector<StackInfo> debug_get_current_stack_info() override {
		TypedArray<Dictionary> ret;
		GDVIRTUAL_REQUIRED_CALL(_debug_get_current_stack_info, ret);
		Vector<StackInfo> sret;
		for (int i = 0; i < ret.size(); i++) {
			StackInfo si;
			Dictionary d = ret[i];
			ERR_CONTINUE(!d.has("file"));
			ERR_CONTINUE(!d.has("func"));
			ERR_CONTINUE(!d.has("line"));
			si.file = d["file"];
			si.func = d["func"];
			si.line = d["line"];
			sret.push_back(si);
		}
		return sret;
	}

	EXBIND0(reload_all_scripts)
	EXBIND2(reload_tool_script, const Ref<Script> &, bool)
	/* LOADER FUNCTIONS */

	GDVIRTUAL0RC(PackedStringArray, _get_recognized_extensions)

	virtual void get_recognized_extensions(List<String> *p_extensions) const override {
		PackedStringArray ret;
		GDVIRTUAL_REQUIRED_CALL(_get_recognized_extensions, ret);
		for (int i = 0; i < ret.size(); i++) {
			p_extensions->push_back(ret[i]);
		}
	}

	GDVIRTUAL0RC(TypedArray<Dictionary>, _get_public_functions)
	virtual void get_public_functions(List<MethodInfo> *p_functions) const override {
		TypedArray<Dictionary> ret;
		GDVIRTUAL_REQUIRED_CALL(_get_public_functions, ret);
		for (int i = 0; i < ret.size(); i++) {
			MethodInfo mi = MethodInfo::from_dict(ret[i]);
			p_functions->push_back(mi);
		}
	}
	GDVIRTUAL0RC(Dictionary, _get_public_constants)
	virtual void get_public_constants(List<Pair<String, Variant>> *p_constants) const override {
		Dictionary ret;
		GDVIRTUAL_REQUIRED_CALL(_get_public_constants, ret);
		for (int i = 0; i < ret.size(); i++) {
			Dictionary d = ret[i];
			ERR_CONTINUE(!d.has("name"));
			ERR_CONTINUE(!d.has("value"));
			p_constants->push_back(Pair<String, Variant>(d["name"], d["value"]));
		}
	}

	EXBIND0(profiling_start)
	EXBIND0(profiling_stop)

	GDVIRTUAL2R(int, _profiling_get_accumulated_data, GDNativePtr<ScriptLanguageExtensionProfilingInfo>, int)

	virtual int profiling_get_accumulated_data(ProfilingInfo *p_info_arr, int p_info_max) override {
		int ret = 0;
		GDVIRTUAL_REQUIRED_CALL(_profiling_get_accumulated_data, p_info_arr, p_info_max, ret);
		return ret;
	}

	GDVIRTUAL2R(int, _profiling_get_frame_data, GDNativePtr<ScriptLanguageExtensionProfilingInfo>, int)

	virtual int profiling_get_frame_data(ProfilingInfo *p_info_arr, int p_info_max) override {
		int ret = 0;
		GDVIRTUAL_REQUIRED_CALL(_profiling_get_accumulated_data, p_info_arr, p_info_max, ret);
		return ret;
	}

	GDVIRTUAL1R(GDNativePtr<void>, _alloc_instance_binding_data, Object *)

	virtual void *alloc_instance_binding_data(Object *p_object) override {
		GDNativePtr<void> ret = nullptr;
		GDVIRTUAL_REQUIRED_CALL(_alloc_instance_binding_data, p_object, ret);
		return ret.operator void *();
	}

	GDVIRTUAL1(_free_instance_binding_data, GDNativePtr<void>)

	virtual void free_instance_binding_data(void *p_data) override {
		GDVIRTUAL_REQUIRED_CALL(_free_instance_binding_data, p_data);
	}

	EXBIND1(refcount_incremented_instance_binding, Object *)
	EXBIND1R(bool, refcount_decremented_instance_binding, Object *)

	EXBIND0(frame)

	EXBIND1RC(bool, handles_global_class_type, const String &)

	GDVIRTUAL1RC(Dictionary, _get_global_class_name, const String &)

	virtual String get_global_class_name(const String &p_path, String *r_base_type = nullptr, String *r_icon_path = nullptr) const override {
		Dictionary ret;
		GDVIRTUAL_REQUIRED_CALL(_get_global_class_name, p_path, ret);
		if (!ret.has("name")) {
			return String();
		}
		if (r_base_type != nullptr && ret.has("base_type")) {
			*r_base_type = ret["base_type"];
		}
		if (r_icon_path != nullptr && ret.has("icon_path")) {
			*r_icon_path = ret["icon_path"];
		}
		return ret["name"];
	}
};

VARIANT_ENUM_CAST(ScriptLanguageExtension::LookupResultType)
VARIANT_ENUM_CAST(ScriptLanguageExtension::CodeCompletionKind)
VARIANT_ENUM_CAST(ScriptLanguageExtension::CodeCompletionLocation)

class ScriptInstanceExtension : public ScriptInstance {
public:
	const GDNativeExtensionScriptInstanceInfo *native_info;
	GDNativeExtensionScriptInstanceDataPtr instance = nullptr;

// There should not be warnings on explicit casts.
#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wignored-qualifiers"
#endif

	virtual bool set(const StringName &p_name, const Variant &p_value) override {
		if (native_info->set_func) {
			return native_info->set_func(instance, (const GDNativeStringNamePtr)&p_name, (const GDNativeVariantPtr)&p_value);
		}
		return false;
	}
	virtual bool get(const StringName &p_name, Variant &r_ret) const override {
		if (native_info->get_func) {
			return native_info->get_func(instance, (const GDNativeStringNamePtr)&p_name, (GDNativeVariantPtr)&r_ret);
		}
		return false;
	}
	virtual void get_property_list(List<PropertyInfo> *p_list) const override {
		if (native_info->get_property_list_func) {
			uint32_t pcount;
			const GDNativePropertyInfo *pinfo = native_info->get_property_list_func(instance, &pcount);
			for (uint32_t i = 0; i < pcount; i++) {
				p_list->push_back(PropertyInfo(Variant::Type(pinfo[i].type), pinfo[i].class_name, PropertyHint(pinfo[i].hint), pinfo[i].hint_string, pinfo[i].usage, pinfo[i].class_name));
			}
			if (native_info->free_property_list_func) {
				native_info->free_property_list_func(instance, pinfo);
			}
		}
	}
	virtual Variant::Type get_property_type(const StringName &p_name, bool *r_is_valid = nullptr) const override {
		if (native_info->get_property_type_func) {
			GDNativeBool is_valid = 0;
			GDNativeVariantType type = native_info->get_property_type_func(instance, (const GDNativeStringNamePtr)&p_name, &is_valid);
			if (r_is_valid) {
				*r_is_valid = is_valid != 0;
			}

			return Variant::Type(type);
		}
		return Variant::NIL;
	}

	virtual Object *get_owner() override {
		if (native_info->get_owner_func) {
			return (Object *)native_info->get_owner_func(instance);
		}
		return nullptr;
	}
	static void _add_property_with_state(const GDNativeStringNamePtr p_name, const GDNativeVariantPtr p_value, void *p_userdata) {
		List<Pair<StringName, Variant>> *state = (List<Pair<StringName, Variant>> *)p_userdata;
		state->push_back(Pair<StringName, Variant>(*(const StringName *)p_name, *(const Variant *)p_value));
	}
	virtual void get_property_state(List<Pair<StringName, Variant>> &state) override {
		if (native_info->get_property_state_func) {
			native_info->get_property_state_func(instance, _add_property_with_state, &state);
		}
	}

	virtual void get_method_list(List<MethodInfo> *p_list) const override {
		if (native_info->get_method_list_func) {
			uint32_t mcount;
			const GDNativeMethodInfo *minfo = native_info->get_method_list_func(instance, &mcount);
			for (uint32_t i = 0; i < mcount; i++) {
				MethodInfo m;
				m.name = minfo[i].name;
				m.flags = minfo[i].flags;
				m.id = minfo[i].id;
				m.return_val = PropertyInfo(Variant::Type(minfo[i].return_value.type), minfo[i].return_value.class_name, PropertyHint(minfo[i].return_value.hint), minfo[i].return_value.hint_string, minfo[i].return_value.usage, minfo[i].return_value.class_name);
				for (uint32_t j = 0; j < minfo[i].argument_count; j++) {
					m.arguments.push_back(PropertyInfo(Variant::Type(minfo[i].arguments[j].type), minfo[i].arguments[j].class_name, PropertyHint(minfo[i].arguments[j].hint), minfo[i].arguments[j].hint_string, minfo[i].arguments[j].usage, minfo[i].arguments[j].class_name));
				}
				const Variant *def_values = (const Variant *)minfo[i].default_arguments;
				for (uint32_t j = 0; j < minfo[i].default_argument_count; j++) {
					m.default_arguments.push_back(def_values[j]);
				}
				p_list->push_back(m);
			}
			if (native_info->free_method_list_func) {
				native_info->free_method_list_func(instance, minfo);
			}
		}
	}
	virtual bool has_method(const StringName &p_method) const override {
		if (native_info->has_method_func) {
			return native_info->has_method_func(instance, (GDNativeStringNamePtr)&p_method);
		}
		return false;
	}

	virtual Variant callp(const StringName &p_method, const Variant **p_args, int p_argcount, Callable::CallError &r_error) override {
		Variant ret;
		if (native_info->call_func) {
			GDNativeCallError ce;
			native_info->call_func(instance, (const GDNativeStringNamePtr)&p_method, (const GDNativeVariantPtr *)p_args, p_argcount, (GDNativeVariantPtr)&ret, &ce);
			r_error.error = Callable::CallError::Error(ce.error);
			r_error.argument = ce.argument;
			r_error.expected = ce.expected;
		}
		return ret;
	}

	virtual void notification(int p_notification) override {
		if (native_info->notification_func) {
			native_info->notification_func(instance, p_notification);
		}
	}
	virtual String to_string(bool *r_valid) override {
		if (native_info->to_string_func) {
			GDNativeBool valid;
			String ret = native_info->to_string_func(instance, &valid);
			if (r_valid) {
				*r_valid = valid != 0;
			}
			return ret;
		}
		return String();
	}

	virtual void refcount_incremented() override {
		if (native_info->refcount_incremented_func) {
			native_info->refcount_incremented_func(instance);
		}
	}
	virtual bool refcount_decremented() override {
		if (native_info->refcount_decremented_func) {
			return native_info->refcount_decremented_func(instance);
		}
		return false;
	}

	virtual Ref<Script> get_script() const override {
		if (native_info->get_script_func) {
			GDNativeObjectPtr script = native_info->get_script_func(instance);
			return Ref<Script>(reinterpret_cast<Script *>(script));
		}
		return Ref<Script>();
	}

	virtual bool is_placeholder() const override {
		if (native_info->is_placeholder_func) {
			return native_info->is_placeholder_func(instance);
		}
		return false;
	}

	virtual void property_set_fallback(const StringName &p_name, const Variant &p_value, bool *r_valid) override {
		if (native_info->set_fallback_func) {
			bool ret = native_info->set_fallback_func(instance, (const GDNativeStringNamePtr)&p_name, (const GDNativeVariantPtr)&p_value);
			if (r_valid) {
				*r_valid = ret;
			}
		}
	}
	virtual Variant property_get_fallback(const StringName &p_name, bool *r_valid) override {
		Variant ret;
		if (native_info->get_fallback_func) {
			bool valid = native_info->get_fallback_func(instance, (const GDNativeStringNamePtr)&p_name, (GDNativeVariantPtr)&ret);
			if (r_valid) {
				*r_valid = valid;
			}
		}
		return ret;
	}

	virtual ScriptLanguage *get_language() override {
		if (native_info->get_language_func) {
			GDNativeExtensionScriptLanguagePtr lang = native_info->get_language_func(instance);
			return reinterpret_cast<ScriptLanguage *>(lang);
		}
		return nullptr;
		;
	}
	virtual ~ScriptInstanceExtension() {
		if (native_info->free_func) {
			native_info->free_func(instance);
		}
	}

#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic pop
#endif
};

#endif // SCRIPT_LANGUAGE_EXTENSION_H
