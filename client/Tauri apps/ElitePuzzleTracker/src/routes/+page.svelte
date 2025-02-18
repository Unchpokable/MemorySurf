<script lang="ts">
    import { onMount } from 'svelte';
    import { invoke } from '@tauri-apps/api/core';
    import { fly } from 'svelte/transition';
    import { FullDump } from '../lib/fulldump';
    import { type JSONValue } from '../lib/jsonvalue';

    let content : JSONValue | null;

    async function getArgs() : Promise<Record<string, string>> {
        return await invoke("get_cli_args");
    }

    function update() {
        if(content != null) {
            
        }
    }

    function onSocketMessage(event : MessageEvent) {
        content = event.data;
    }

    function onSocketOpen(event : Event) {
        content = null;
    }

    function onSocketClose(event : CloseEvent) {
        content = null;
    }

    function onSocketError(event : Event) {
        content = null;
    }

    onMount(() => {
        let webSocket: WebSocket | null = null;

        async function setupWebSocket() {
            try {
                const args: Record<string, string> = await getArgs();
                const ws = args.ws;
                if (!ws) {
                    console.error('Параметр ws не найден');
                    return;
                }
                webSocket = new WebSocket(`ws://localhost:${ws}`);
                webSocket.addEventListener('message', onSocketMessage);
                webSocket.addEventListener('open', onSocketOpen);
                webSocket.addEventListener('close', onSocketClose);
                webSocket.addEventListener('error', onSocketError);
            } catch (error) {
                console.error('Ошибка при получении аргументов:', error);
            }
        }

        setupWebSocket();

        return () => {
            if (webSocket) {
            webSocket.close();
            }
        };
    });

</script>

<div class="content">
    <table>

    </table>
</div>

<style>
    @font-face{
        font-family: "Exo-Bold";
        src: url("/fonts/ExoSoft-Bold");
        font-weight: normal;
        font-style: normal;
    }
    @font-face{
        font-family: "Exo-SemiBold";
        src: url("/fonts/ExoSoft-SemiBold");
        font-weight: normal;
        font-style: normal;
    }
    @font-face{
        font-family: "Exo-Normal";
        src: url("/fonts/ExoSoft-Regular");
        font-weight: normal;
        font-style: normal;
    }

    .content {
        font-family: "Exo-Bold";
    }
</style>
