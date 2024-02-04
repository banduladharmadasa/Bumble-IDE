window.onresize = onResize;

function onResize(e) {
    var left = 0;
    var width = document.documentElement.clientWidth;
    var height = document.documentElement.clientHeight;

    width -= left;
    var container = document.getElementById('editor');
    container.style.width = width + 'px';
    container.style.height = height + 'px';
    container.style.left = left + 'px';
    env.split.resize();
}

var modelist = ace.require('ace/ext/modelist');
var filePath = '%s';
var mode = modelist.getModeForPath(filePath).mode;
var theme = '%s';
var UndoManager = ace.require('ace/undomanager').UndoManager;
var inlineEnv = {}
var Split = ace.require('ace/split').Split;
var container = document.getElementById('editor');
var split = new Split(container, theme, 1);
var editor = split.getEditor(0);
inlineEnv.editor = editor;
editor.session.setUndoManager(new UndoManager());
inlineEnv.editor.setOption('enableEmmet', true);
inlineEnv.editor.setOptions({ enableBasicAutocompletion: true, enableSnippets: true, enableLiveAutocompletion: true });
editor.session.setMode(mode);
inlineEnv.split = split;
window.env = inlineEnv;
function setContent(text) {
    editor.setValue(text);
    editor.clearSelection();
}
function setTheme(theme) {
    editor.setTheme(theme);
    return true;
}
function executeEditorCommand(command) {
    editor.execCommand(command)
}
;
var LineWidgets = ace.require('ace/line_widgets').LineWidgets;
var Editor = ace.require('ace/editor').Editor;
var Renderer = ace.require('ace/virtual_renderer').VirtualRenderer;
var dom = ace.require('ace/lib/dom');
inlineEnv.running = false; editor.commands.addCommand({
    name: 'openInlineEditor',
    bindKey: 'null',
    exec: function (editor) {
        inlineEnv.running = true; var split = window.inlineEnv.split;
        var s = editor.session;
        var inlineEditor = new Editor(new Renderer());
        var splitSession = split.$cloneSession(s);
        var row = editor.getCursorPosition().row;
        if (editor.session.lineWidgets && editor.session.lineWidgets[row]) {
            editor.session.lineWidgets[row].destroy();
            return;
        }

        var rowCount = 10;
        var w = {
            row: row,
            fixedWidth: true,
            el: dom.createElement('div'),
            editor: inlineEditor
        };
        var el = w.el;
        el.appendChild(inlineEditor.container);
        if (!editor.session.widgetManager) {
            editor.session.widgetManager = new LineWidgets(editor.session);
            editor.session.widgetManager.attach(editor);
        }

        var h = rowCount * editor.renderer.layerConfig.lineHeight;
        inlineEditor.container.style.height = h + 'px';
        el.style.position = 'absolute';
        el.style.zIndex = '4';
        el.style.borderTop = 'solid orange 12px';
        el.style.borderBottom = 'solid orange 2px';

        inlineEditor.setSession(splitSession);
        editor.session.widgetManager.addLineWidget(w);

        var kb = {
            handleKeyboard: function (_, hashId, keyString) {
                if (hashId === 0 && keyString === 'esc') {
                    inlineEnv.destroy();
                    return true;
                }
            }
        };

        inlineEnv.destroy = function () {
            editor.keyBinding.removeKeyboardHandler(kb);
            s.widgetManager.removeLineWidget(w);
        };

        editor.keyBinding.addKeyboardHandler(kb);
        inlineEditor.keyBinding.addKeyboardHandler(kb);
        inlineEditor.setTheme('ace/theme/solarized_light');
    }
});


window.define = ace.define;
window.require = ace.require;
