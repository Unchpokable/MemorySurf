// Learn more about Tauri commands at https://tauri.app/develop/calling-rust/

use std::collections::HashMap;

#[tauri::command]
fn get_cli_args() -> HashMap<String, String> {
    let mut args = HashMap::new();

    for arg in std::env::args().skip(1) {
        if arg.starts_with("--") {
            let trimmed = &arg[2..];

            let parts: Vec<&str> = trimmed.splitn(2, '=').collect();

            if parts.len() == 2 {
                args.insert(parts[0].to_string(), parts[1].to_string());
            } else {
                args.insert(parts[0].to_string(), String::new());
            }
        }
    }

    return args;
}

#[cfg_attr(mobile, tauri::mobile_entry_point)]
pub fn run() {
    tauri::Builder::default()
        .invoke_handler(tauri::generate_handler![get_cli_args])
        .plugin(tauri_plugin_opener::init())
        .run(tauri::generate_context!())
        .expect("error while running tauri application");
}
