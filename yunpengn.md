# yunpengn
###### \java\seedu\address\commons\events\model\TagColorChangedEvent.java
``` java
/**
 * Indicates the color of some tag(s) has been changed.
 */
public class TagColorChangedEvent extends BaseEvent {
    private Tag tag;
    private String color;

    public TagColorChangedEvent(Tag tag, String color) {
        this.tag = tag;
        this.color = color;
    }

    @Override
    public String toString() {
        return String.format(ChangeTagColorCommand.MESSAGE_SUCCESS, tag, color);
    }
}
```
###### \java\seedu\address\commons\util\FileUtil.java
``` java
    /**
     * Checks whether the given file is an image (according to its MIME type).<br>
     *
     * Notice: This check is not accurate and may lead to security problems.
     */
    public static boolean isImage(File file) {
        String type = TYPE_MAP.getContentType(file);
        return type.split("/")[0].equals("image")
                // To allow the image downloaded from Internet, especially png file.
                || type.equals("application/octet-stream");
    }
```
###### \java\seedu\address\commons\util\JsonUtil.java
``` java
    /**
     * Read JSON data from a given URL and convert the data to an instance of the given class.
     *
     * @param url is the URL to the remote JSON data.
     */
    public static <T> T fromJsonUrl(URL url, Class<T> instanceClass) throws IOException {
        return objectMapper.readValue(url, instanceClass);
    }
```
###### \java\seedu\address\commons\util\UrlUtil.java
``` java
/**
 * Helps with parsing a given URL and obtain GET parameters from it.
 */
public class UrlUtil {
    public static URL parseUrlString(String url) throws MalformedURLException {
        return new URL(url);
    }

    /**
     * Fetches all GET parameters from a given {@link URL} object. It is assumed that there is no GET parameter with
     * the same key in the {@code url}. If there is, only the last one (among those parameters with the same name)
     * will be included in the returned {@link Map}. See also {@link #fetchUrlParameterKeys(URL)}.
     *
     * @param url is a given {@link URL} object.
     *
     * @return a {@link Map} containing all key-value pairs of GET parameters in the given {@code url}.
     */
    public static Map<String, String> fetchUrlParameters(URL url) throws UnsupportedEncodingException {
        String query = urlDecode(url.getQuery());

        if (Strings.isNullOrEmpty(query)) {
            return Collections.emptyMap();
        }

        Map<String, String> pairs = new HashMap<>();
        for (String pair: query.split("&")) {
            int index = pair.indexOf("=");
            pairs.put(pair.substring(0, index), pair.substring(index + 1));
        }

        return pairs;
    }

    /**
     * Fetches the keys all GET parameters from a given {@link URL} object. It is assumed that there is no GET parameter
     * with the same key in the {@code url}. If there is, only the last one (among those parameters with the same name)
     * will be included in the returned {@link Set}. See also {@link #fetchUrlParameters(URL)}.
     *
     * @param url is a given {@link URL} object.
     *
     * @return a {@link Set} containing all keys of GET parameters in the given {@code url}.
     */
    public static Set<String> fetchUrlParameterKeys(URL url) throws UnsupportedEncodingException {
        String query = urlDecode(url.getQuery());

        if (Strings.isNullOrEmpty(query)) {
            return Collections.emptySet();
        }

        String[] pairs = query.split("&");
        return Arrays.stream(pairs).map(pair -> pair.substring(0, pair.indexOf("="))).collect(Collectors.toSet());
    }

    public static String urlDecode(String url) throws UnsupportedEncodingException {
        return URLDecoder.decode(url, "utf-8");
    }
}
```
###### \java\seedu\address\logic\commands\configs\AddPropertyCommand.java
``` java
/**
 * Adds a new property to the application.
 */
public class AddPropertyCommand extends ConfigCommand {
    public static final String MESSAGE_USAGE = "Example: " + COMMAND_WORD + " --add-property "
            + "s/b f/birthday m/Birthday needs to be a valid date format "
            + "r/^(0[1-9]|[12][0-9]|3[01])(0[1-9]|1[012])[0-9]{4}";
    public static final String MESSAGE_SUCCESS = "Added a new property: %1$s";

    static final String MESSAGE_DUPLICATE_PROPERTY =
            "Another property with the same short name already exists in the application.";
    static final String MESSAGE_INVALID_REGEX = "The regular expression you provided is invalid.";

    private final String shortName;
    private final String fullName;
    private final String constraintMessage;
    private final String regex;

    public AddPropertyCommand(String configValue, String shortName, String fullName, String message, String regex) {
        super(ADD_PROPERTY, configValue);
        this.shortName = shortName;
        this.fullName = fullName;
        this.constraintMessage = message;
        this.regex = regex;
    }

    @Override
    public CommandResult execute() throws CommandException {
        try {
            model.addProperty(shortName, fullName, constraintMessage, regex);
            return new CommandResult(String.format(MESSAGE_SUCCESS, configValue));
        } catch (DuplicatePropertyException dpe) {
            throw new CommandException(MESSAGE_DUPLICATE_PROPERTY);
        } catch (PatternSyntaxException pse) {
            throw new CommandException(MESSAGE_INVALID_REGEX);
        }
    }
}
```
###### \java\seedu\address\logic\commands\configs\ChangeTagColorCommand.java
``` java
/**
 * Changes the color of an existing tag.
 */
public class ChangeTagColorCommand extends ConfigCommand {
    public static final String MESSAGE_SUCCESS = "The color of tag %1$s has been changed to %2$s.";
    public static final String MESSAGE_USAGE =  "Example: " + COMMAND_WORD + " --set-tag-color "
            + "friends blue";
    private static final String MESSAGE_NO_SUCH_TAG = "There is no such tag.";

    private Tag tag;
    private String newColor;

    public ChangeTagColorCommand(String configValue, String tagName, String tagColor) throws ParseException {
        super(TAG_COLOR, configValue);

        try {
            /* Two tags are equal as long as their tagNames are the same. */
            tag = new Tag(tagName);
        } catch (IllegalValueException e) {
            throw new ParseException(MESSAGE_TAG_CONSTRAINTS);
        }
        this.newColor = tagColor;
    }

    @Override
    public CommandResult execute() throws CommandException {
        if (!model.hasTag(tag)) {
            throw new CommandException(MESSAGE_NO_SUCH_TAG);
        }

        model.setTagColor(tag, newColor);
        return new CommandResult(String.format(MESSAGE_SUCCESS, tag, newColor));
    }
}
```
###### \java\seedu\address\logic\commands\configs\ConfigCommand.java
``` java
/**
 * Customizes the configuration of the application.
 */
public abstract class ConfigCommand extends Command {
    public static final String COMMAND_WORD = "config";
    public static final String COMMAND_ALIAS = "cfg";

    public static final String MESSAGE_USAGE = COMMAND_WORD + ": Changes the configuration of the application. "
            + "Parameters: " + "--CONFIG_TYPE "
            + "NEW_CONFIG_VALUE\n"
            + "Example: " + COMMAND_WORD + " --set-tag-color "
            + "friends #9381a0";

    public static final String MESSAGE_SUCCESS = "Configuration changed: %1$s";

    /**
     * Different types of sub-commands within {@link ConfigCommand}.
     */
    public enum ConfigType {
        ADD_PROPERTY, TAG_COLOR
    }

    public static final HashMap<String, ConfigType> TO_ENUM_CONFIG_TYPE = new HashMap<>();

    static {
        TO_ENUM_CONFIG_TYPE.put("add-property", ConfigType.ADD_PROPERTY);
        TO_ENUM_CONFIG_TYPE.put("set-tag-color", ConfigType.TAG_COLOR);
    }

    protected String configValue;

    private ConfigType configType;

    public ConfigCommand(ConfigType configType, String configValue) {
        this.configType = configType;
        this.configValue = configValue;
    }

    @Override
    public boolean equals(Object other) {
        return other == this // short circuit if same object
                || (other instanceof ConfigCommand // instanceof handles nulls
                && configType.equals(((ConfigCommand) other).configType)
                && configValue.equals(((ConfigCommand) other).configValue));
    }
}
```
###### \java\seedu\address\logic\commands\imports\ImportCommand.java
``` java
/**
 * Imports data from various format to the application.
 */
public abstract class ImportCommand extends UndoableCommand {
    public static final String COMMAND_WORD = "import";
    public static final String COMMAND_ALIAS = "i";

    public static final String MESSAGE_USAGE = COMMAND_WORD
            + ": Imports data from various locations in various formats.\n"
            + "Examples:\n"
            + COMMAND_WORD + " --script C:\\Users\\John Doe\\Documents\\bonus.bo (Windows)\n"
            + COMMAND_WORD + " /Users/John Doe/Documents/bonus.xml (macOS, Linux)\n";

    public static final String MESSAGE_IMPORT_SUCCESS = "Imported data from: %1$s";
    public static final String MESSAGE_PROBLEM_READING_FILE = "There is a problem when the application tried to"
            + " read the given file. Please check the file permission.";
    public static final String MESSAGE_NOT_XML_FILE = "According to the extension, the file is not a valid XML "
            + "file.\nYou need to specify with explicit parameter if you want to use other formats.";
    public static final String MESSAGE_NOT_BO_FILE = "According to the extension, the file is not a valid BoNUS"
            + "script file (should end with .bo).";

```
###### \java\seedu\address\logic\commands\imports\ImportCommand.java
``` java
    /**
     * Different types of sub-commands within {@link ImportCommand}.
     */
    public enum ImportType {
        XML, SCRIPT, NUSMODS
    }

    public static final HashMap<String, ImportType> TO_ENUM_IMPORT_TYPE = new HashMap<>();

    static {
        TO_ENUM_IMPORT_TYPE.put("xml", ImportType.XML);
        TO_ENUM_IMPORT_TYPE.put("script", ImportType.SCRIPT);
        TO_ENUM_IMPORT_TYPE.put("nusmods", ImportType.NUSMODS);
    }

    protected String path;

    private ImportType importType;

    public ImportCommand(String path, ImportType importType) {
        this.path = path;
        this.importType = importType;
    }

    @Override
    public boolean equals(Object other) {
        return other == this // short circuit if same object
                || (other instanceof ImportCommand // instanceof handles nulls
                && importType.equals(((ImportCommand) other).importType)
                && path.equals(((ImportCommand) other).path));
    }
}
```
###### \java\seedu\address\logic\commands\imports\ImportNusmodsCommand.java
``` java
/**
 * Imports data from the URL of a NUSMods timetable.
 *
 * @see <a href="https://nusmods.com/">https://nusmods.com/</a>
 */
public class ImportNusmodsCommand extends ImportCommand {
    /* Messages displayed to the user (ready for use). */
    public static final String MESSAGE_USAGE = COMMAND_WORD
            + ": Imports data from NUSMods timetable URL.\n"
            + "Examples:\n"
            + COMMAND_WORD + " --nusmods https://nusmods.com/timetable/2017-2018/sem1?CS2103T[TUT]=C01";
    public static final String INVALID_URL = "The URL provided is not from NUSMods website. \n%1$s";
    public static final String YEAR_OFFSET_BY_ONE =
            "The start/end year of the same academic year must offset by 1";
    public static final String MESSAGE_SUCCESS = "%1$d examinations have been added as events.";

    private static final String YEAR_INVALID =
            "Maybe you modify the part regarding academic year and semester.";
    private static final String INVALID_ENCODING = "The URL encoding is not supported. Please use UTF-8.";
    private static final String MODULE_INFO_JSON_URL_FORMAT =
            "http://api.nusmods.com/%1$s-%2$s/%3$s/modules/%4$s.json";
    private static final String UNABLE_FETCH_MODULE_INFO = "Unable to fetch the information of module %1$s.";
    private static final String EXAM_EVENT_NAME = "%1$s Examination";
    private static final String UNABLE_CREATE_EXAM_EVENT = "Unable to create exam event for %1$s.";
    private static final String EXAM_EVENT_DEFAULT_ADDRESS = "NUS";
    private static final String EXAM_EVENT_EXIST_DUPLICATE =
            "The examination event for %1$s already exists in the application.";

    private static final String SOME_EXAMS_NOT_ADDED =
            "\nHowever, some examination were not added since they already exist in the application.";
    private static final String TO_STRING_FORMAT = "AY%1$d-%2$d Semester %3$d";

    // Semester should be a one-digit number from 1 to 4, year must be after 2000.
    private static final Pattern URL_SEMESTER_INFO_FORMAT  =
            Pattern.compile("/timetable/(?<year1>20\\d{2})[-](?<year2>20\\d{2})/sem(?<semester>[1-4])");

    private static final Logger logger = LogsCenter.getLogger(ImportNusmodsCommand.class);

    private URL url;
    private int yearStart;
    private int yearEnd;
    private int semester;

    // A counter for how many examinations have been added to the application as events.
    private int eventsAdded;
    // Signals whether we fail to add any examination.
    private boolean failToAdd;

    /**
     * Only checks the academic year and semester information in the constructor. If the query part (module information)
     * is wrong, they will simply not be added to event when executed.
     */
    public ImportNusmodsCommand(URL url) throws ParseException {
        super(url.toString(), ImportType.NUSMODS);
        this.url = url;
        matchSemesterInformation();
        eventsCenter.registerHandler(this);
        eventsAdded = 0;
        failToAdd = false;
    }

    @Override
    public CommandResult executeUndoableCommand() throws CommandException {
        // Get all the module codes.
        Set<String> modules;
        try {
            modules = fetchModuleCodes();
        } catch (UnsupportedEncodingException e) {
            throw new CommandException(String.format(INVALID_URL, INVALID_ENCODING));
        }

        // Retrieve information of all modules from NUSMods JSON API.
        Set<ModuleInfo> moduleInfo = new HashSet<>();
        for (String moduleCode: modules) {
            try {
                moduleInfo.add(getModuleInfo(moduleCode));
            } catch (IOException e) {
                throw new CommandException(String.format(UNABLE_FETCH_MODULE_INFO, moduleCode));
            }
        }

        // Add an event for the final examination of each module.
        for (ModuleInfo module: moduleInfo) {
            addExamEvent(module).ifPresent(e -> {
                try {
                    model.addEvent(e);
                    incrementEventsAddedCount();
                } catch (DuplicateEventException e1) {
                    failToAdd = true;
                    logger.info(String.format(EXAM_EVENT_EXIST_DUPLICATE, module.getModuleCode()));
                }
            });
        }

        // Do not switch to event listing if there is no change.
        if (eventsAdded > 0) {
            raise(new SwitchToEventsListEvent());
        }

        String successMessage = String.format(MESSAGE_SUCCESS, eventsAdded);
        if (failToAdd) {
            return new CommandResult(successMessage + SOME_EXAMS_NOT_ADDED);
        } else {
            return new CommandResult(successMessage);
        }
    }

    /**
     * Extracts and matches the academic year and semester information from user input URL.
     *
     * @throws ParseException if the given URL does not obey NUSMods convention.
     */
    private void matchSemesterInformation() throws ParseException {
        Matcher matcher = URL_SEMESTER_INFO_FORMAT.matcher(url.getPath());
        if (!matcher.matches()) {
            throw  new ParseException(String.format(INVALID_URL, ""));
        }

        try {
            yearStart = Integer.parseInt(matcher.group("year1"));
            yearEnd = Integer.parseInt(matcher.group("year2"));
            semester = Integer.parseInt(matcher.group("semester"));
        } catch (NumberFormatException nfe) {
            throw new ParseException(String.format(INVALID_URL, YEAR_INVALID));
        }

        if (yearEnd - yearStart != 1) {
            throw new ParseException(String.format(INVALID_URL, YEAR_OFFSET_BY_ONE));
        }
    }

    /**
     * Returns all the module codes embedded in the {@link #url} field. This fetching will ignore the type
     * of sessions (lecture, tutorial, etc.). Thus, it only returns all modules in the {@link #url}.
     */
    private Set<String> fetchModuleCodes() throws UnsupportedEncodingException {
        Set<String> keys = UrlUtil.fetchUrlParameterKeys(url);
        return keys.stream().map(key -> key.substring(0, key.indexOf("["))).collect(Collectors.toSet());
    }

    /**
     * Gets the module information from NUSMods API and convert to a {@link ModuleInfo} class.
     *
     * @see <a href="https://github.com/nusmodifications/nusmods-api#get-acadyearsemestermodulesmodulecodejson">
     *     NUSMods API official documentation</a>
     */
    private ModuleInfo getModuleInfo(String moduleCode) throws IOException {
        URL url = new URL(String.format(MODULE_INFO_JSON_URL_FORMAT, yearStart, yearEnd, semester, moduleCode));
        return JsonUtil.fromJsonUrl(url, ModuleInfo.class);
    }

    /**
     * Creates an {@link Event} representing the final examination according to information from {@link ModuleInfo}.
     *
     * @return an {@link Optional} that is present with an {@link Event} only if the given module has a final
     * examination (some modules at NUS are 100% continuous-assessment, like CFG1010).
     */
    private Optional<ReadOnlyEvent> addExamEvent(ModuleInfo module) throws CommandException {
        if (module.getExamDate() == null) {
            return Optional.empty();
        }

        try {
            Name eventName = new Name(String.format(EXAM_EVENT_NAME, module.getModuleCode()));
            DateTime eventDatetime = new DateTime(module.getExamDate());
            Address eventAddress = new Address(EXAM_EVENT_DEFAULT_ADDRESS);
            return Optional.of(new Event(eventName, eventDatetime, eventAddress));
        } catch (IllegalValueException | PropertyNotFoundException e) {
            throw new CommandException(String.format(UNABLE_CREATE_EXAM_EVENT, module.getModuleCode()));
        }
    }

    private void incrementEventsAddedCount() {
        eventsAdded++;
    }

    @Override
    public String toString() {
        return String.format(TO_STRING_FORMAT, yearStart, yearEnd, semester);
    }
}
```
###### \java\seedu\address\logic\commands\imports\ModuleInfo.java
``` java
/**
 * A Java class representation of module information from NUSMods JSON API.
 *
 * @see <a href="https://github.com/nusmodifications/nusmods-api#get-acadyearsemestermodulesmodulecodejson">
 *     NUSMods API official documentation</a>
 */
public class ModuleInfo {
    private String moduleCode;
    private String moduleTitle;
    private int moduleCredit;
    private Date examDate;

    public String getModuleCode() {
        return moduleCode;
    }

    public Date getExamDate() {
        return examDate;
    }

    @Override
    public boolean equals(Object other) {
        if (other == this) {
            return true;
        } else if (!(other instanceof ModuleInfo)) {
            // This handles null as well.
            return false;
        } else {
            ModuleInfo o = (ModuleInfo) other;
            return Objects.equals(moduleCode, o.moduleCode);
        }
    }

    /**
     * Each {@link #moduleCode} should link to <b>one and only one</b> {@link ModuleInfo} object.
     */
    @Override
    public int hashCode() {
        return moduleCode.hashCode();
    }

    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder();
        sb.append("Module Code: " + moduleCode);
        sb.append("\nModule Title: " + moduleTitle);
        sb.append("\nModule Credit: " + moduleCredit);
        sb.append("\nExamination Date: " + examDate);
        return sb.toString();
    }
}
```
###### \java\seedu\address\logic\commands\person\AddAvatarCommand.java
``` java
/**
 * Adds an {@link Avatar} to the selected person.
 */
public class AddAvatarCommand extends UndoableCommand {
    public static final String COMMAND_WORD = "avatar";
    public static final String COMMAND_ALIAS = "avr";

    public static final String MESSAGE_USAGE = COMMAND_WORD
            + ": Adds avatar to the person identified by the index number used in the last person listing.\n"
            + "Parameters: INDEX (must be a positive integer) IMAGE_PATH\n"
            + "Example: " + COMMAND_WORD + " 1 something.png";

    public static final String MESSAGE_ADD_AVATAR_SUCCESS = "Added avatar to person: %1$s";

    private final Index targetIndex;
    private final Avatar avatar;

    public AddAvatarCommand(Index targetIndex, Avatar avatar) {
        this.targetIndex = targetIndex;
        this.avatar = avatar;
    }

    @Override
    protected CommandResult executeUndoableCommand() throws CommandException {
        List<ReadOnlyPerson> lastShownList = model.getFilteredPersonList();

        if (targetIndex.getZeroBased() >= lastShownList.size()) {
            throw new CommandException(Messages.MESSAGE_INVALID_PERSON_DISPLAYED_INDEX);
        }

        ReadOnlyPerson person = lastShownList.get(targetIndex.getZeroBased());
        model.setPersonAvatar(person, avatar);

        return new CommandResult(String.format(MESSAGE_ADD_AVATAR_SUCCESS, person));
    }

    @Override
    public boolean equals(Object other) {
        return other == this // short circuit if same object
                || (other instanceof AddAvatarCommand // instanceof handles nulls
                && this.targetIndex.equals(((AddAvatarCommand) other).targetIndex)
                && this.avatar.equals(((AddAvatarCommand) other).avatar)); // state check
    }
}
```
###### \java\seedu\address\logic\parser\ConfigCommandParser.java
``` java
/**
 * Parses input arguments and creates a new ConfigCommand object
 */
public class ConfigCommandParser implements Parser<ConfigCommand> {
    // Some messages ready to use.
    public static final String CONFIG_TYPE_NOT_FOUND = "The configuration you want to change is not "
            + "available or the command entered is incomplete.";
    public static final String COLOR_CODE_WRONG = "The color must be one of the pre-defined color names or "
            + "a valid hexadecimal RGB value";
    private static final String MESSAGE_REGEX_TOGETHER = "Constraint message and regular expression must be "
            + "both present or absent";

    /* Regular expressions for validation. ArgumentMultiMap not applicable here. */
    private static final Pattern CONFIG_COMMAND_FORMAT = Pattern.compile("--(?<configType>\\S+)(?<configValue>.+)");
    private static final Pattern TAG_COLOR_FORMAT = Pattern.compile("(?<tagName>\\p{Alnum}+)\\s+(?<tagNewColor>.+)");
    // A valid RGB value should be 3-bit or 6-bit hexadecimal number.
    private static final Pattern RGB_FORMAT = Pattern.compile("#([A-Fa-f0-9]{6}|[A-Fa-f0-9]{3})");
    // Only contains alphabets (a-z or A-Z)
    private static final Pattern ONLY_ALPHABET = Pattern.compile("[a-zA-Z]+");

    @Override
    public ConfigCommand parse(String args) throws ParseException {
        requireNonNull(args);

        // Defensive programming here to use trim again.
        final Matcher matcher = CONFIG_COMMAND_FORMAT.matcher(args.trim());
        if (!matcher.matches()) {
            throw new ParseException(String.format(MESSAGE_INVALID_COMMAND_FORMAT, ConfigCommand.MESSAGE_USAGE));
        }

        final String configType = matcher.group("configType").trim();
        if (!checkConfigType(configType)) {
            throw new ParseException(String.format(MESSAGE_INVALID_COMMAND_FORMAT, CONFIG_TYPE_NOT_FOUND));
        }

        final ConfigType enumConfigType = toEnumType(configType);
        final String configValue = matcher.group("configValue").trim();

        return checkConfigValue(enumConfigType, configValue);
    }

    private boolean checkConfigType(String type) {
        return ConfigCommand.TO_ENUM_CONFIG_TYPE.containsKey(type);
    }

    private ConfigType toEnumType(String type) {
        return ConfigCommand.TO_ENUM_CONFIG_TYPE.get(type);
    }

    /**
     * Validates the input for different {@link ConfigType} and creates an {@link ConfigCommand} accordingly.
     */
    private ConfigCommand checkConfigValue(ConfigType enumConfigType, String value) throws ParseException {
        switch (enumConfigType) {
        case ADD_PROPERTY:
            return checkAddProperty(value);
        case TAG_COLOR:
            return checkTagColor(value);
        default:
            System.err.println("Unknown ConfigType. Should never come to here.");
            throw new ParseException(String.format(MESSAGE_INVALID_COMMAND_FORMAT, CONFIG_TYPE_NOT_FOUND));
        }
    }

    /**
     * Creates an {@link ChangeTagColorCommand}.
     */
    private ChangeTagColorCommand checkTagColor(String value) throws ParseException {
        Matcher matcher = TAG_COLOR_FORMAT.matcher(value.trim());
        if (!matcher.matches()) {
            throw new ParseException(String.format(MESSAGE_INVALID_COMMAND_FORMAT,
                    ChangeTagColorCommand.MESSAGE_USAGE));
        }

        // Get the tag name and the customize new color for that tag.
        final String tagName = matcher.group("tagName").trim();
        String tagColor = matcher.group("tagNewColor").trim();

        // Checks whether the given color is valid.
        if (!isValidColorCode(tagColor)) {
            throw new ParseException(String.format(MESSAGE_INVALID_COMMAND_FORMAT, COLOR_CODE_WRONG));
        }

        return new ChangeTagColorCommand(value, tagName, tagColor);
    }

    /**
     * Creates an {@link AddPropertyCommand}.
     */
    private AddPropertyCommand checkAddProperty(String value) throws ParseException {
        /*
        * Hack here: ArgumentTokenizer requires a whitespace before each prefix to count for an occurrence. Thus, we
        * have to explicitly add a whitespace before the string so as to successfully extract the first prefix.
        */
        ArgumentMultimap argMultimap = ArgumentTokenizer.tokenize(" " + value,
                PREFIX_SHORT_NAME, PREFIX_FULL_NAME, PREFIX_MESSAGE, PREFIX_REGEX);

        // shortName and fullName must be supplied by the user.
        if (!ParserUtil.arePrefixesPresent(argMultimap, PREFIX_SHORT_NAME, PREFIX_FULL_NAME)) {
            throw new ParseException(String.format(MESSAGE_INVALID_COMMAND_FORMAT,
                    AddPropertyCommand.MESSAGE_USAGE));
        }
        String shortName = argMultimap.getValue(PREFIX_SHORT_NAME).get();
        String fullName = capitalize(argMultimap.getValue(PREFIX_FULL_NAME).get());

        // message and regex must be supplied together or both be absent.
        if (!(ParserUtil.arePrefixesPresent(argMultimap, PREFIX_MESSAGE, PREFIX_REGEX)
                || ParserUtil.arePrefixesAbsent(argMultimap, PREFIX_MESSAGE, PREFIX_REGEX))) {
            throw new ParseException(String.format(MESSAGE_INVALID_COMMAND_FORMAT, MESSAGE_REGEX_TOGETHER));
        }
        String message = argMultimap.getValue(PREFIX_MESSAGE).orElse(String.format(DEFAULT_MESSAGE, fullName));
        String regex = argMultimap.getValue(PREFIX_REGEX).orElse(DEFAULT_REGEX);

        return new AddPropertyCommand(value, shortName, fullName, message, regex);
    }

    /**
     * Checks whether the given string is a valid RGB value or a fully-alphabetical string (we do not check whether it
     * is one of the 140 pre-defined CSS color names).
     *
     * TODO: Search for any API to check whether it is one of 140 pre-defined names.
     *
     * @see <a href=https://docs.oracle.com/javafx/2/api/javafx/scene/doc-files/cssref.html#typecolor>
     *     JavaFX CSS Reference Guide</a>
     */
    private boolean isValidColorCode(String color) {
        // Either all letters or a valid RGB value.
        return ONLY_ALPHABET.matcher(color).matches() || RGB_FORMAT.matcher(color).matches();

    }

    /**
     * Converts the first letter in {@code str} to upper-case (only if it starts with an alphabet).
     */
    private String capitalize(String original) {
        if (original == null || original.length() == 0) {
            return original;
        }

        return original.substring(0, 1).toUpperCase() + original.substring(1);
    }
}
```
###### \java\seedu\address\logic\parser\ImportCommandParser.java
``` java
/**
 * Parses input arguments and creates a new sub-command of {@link ImportCommand} object.
 */
public class ImportCommandParser implements Parser<ImportCommand> {
    // Some messages ready to use.
    public static final String IMPORT_TYPE_NOT_FOUND = "The format of the data you want to import is "
            + "currently not supported";

    /* Regular expressions for validation. */
    private static final Pattern IMPORT_COMMAND_FORMAT = Pattern.compile("--(?<importType>\\S+)\\s+(?<path>.+)");
    private static final Pattern IMPORT_NUSMODS_FORMAT =
            Pattern.compile("https?://(www.)?nusmods.com/timetable/\\S*");
    private static final String ARG_BEGIN_WITH = "--";
    private static final String IMPORT_DEFAULT_TYPE = "--xml ";

    @Override
    public ImportCommand parse(String args) throws ParseException {
        requireNonNull(args);
        args = args.trim();

        // Be default, import from .xml file if not specified by the user.
        if (!args.startsWith(ARG_BEGIN_WITH)) {
            args = IMPORT_DEFAULT_TYPE + args;
        }

        // Matches the import file type and import file path.
        final Matcher matcher = IMPORT_COMMAND_FORMAT.matcher(args);
        if (!matcher.matches()) {
            throw new ParseException(String.format(MESSAGE_INVALID_COMMAND_FORMAT, ImportCommand.MESSAGE_USAGE));
        }

        final String importType = matcher.group("importType").trim();
        if (!checkImportType(importType)) {
            throw new ParseException(String.format(MESSAGE_INVALID_COMMAND_FORMAT, IMPORT_TYPE_NOT_FOUND));
        }

        final ImportType enumImportType = toEnumType(importType);
        final String path = matcher.group("path").trim();

        return checkImportPath(enumImportType, path);
    }

    private boolean checkImportType(String type) {
        return ImportCommand.TO_ENUM_IMPORT_TYPE.containsKey(type);
    }

    private ImportType toEnumType(String type) {
        return ImportCommand.TO_ENUM_IMPORT_TYPE.get(type);
    }

    /**
     * Validates the input for different {@link ImportType} and creates an {@link ImportCommand} accordingly.
     */
    private ImportCommand checkImportPath(ImportType enumImportType, String path) throws ParseException {
        switch (enumImportType) {
        case XML:
            return checkXmlImport(path);
        case SCRIPT:
            return checkScriptImport(path);
        case NUSMODS:
            return checkNusmodsImport(path);
        default:
            System.err.println("Unknown ImportType. Should never come to here.");
            throw new ParseException(String.format(MESSAGE_INVALID_COMMAND_FORMAT, IMPORT_TYPE_NOT_FOUND));
        }
    }

```
###### \java\seedu\address\logic\parser\ImportCommandParser.java
``` java
    private ImportCommand checkScriptImport(String path) {
        return null;
    }

    /**
     * Creates an {@link ImportNusmodsCommand}.
     */
    private ImportCommand checkNusmodsImport(String path) throws ParseException {
        /*
         * We only do a simple matching check here. More detailed checking will be done when
         * the {@link ImportNusmodsCommand} is constructed or executed. The matching here
         * only serves as a defensive programming purpose.
         */
        if (!IMPORT_NUSMODS_FORMAT.matcher(path).matches()) {
            throw new ParseException(String.format(MESSAGE_INVALID_COMMAND_FORMAT,
                    ImportNusmodsCommand.MESSAGE_USAGE));
        }

        try {
            return new ImportNusmodsCommand(new URL(path));
        } catch (MalformedURLException e) {
            throw new ParseException(String.format(MESSAGE_INVALID_COMMAND_FORMAT,
                    ImportNusmodsCommand.MESSAGE_USAGE));
        }
    }
}
```
###### \java\seedu\address\logic\parser\person\AddAvatarCommandParser.java
``` java
/**
 * Parses input arguments and creates a new {@link AddAvatarCommand} object.
 */
public class AddAvatarCommandParser implements Parser<AddAvatarCommand> {
    /* Regular expressions for validation. ArgumentMultiMap not applicable here. */
    private static final Pattern COMMAND_FORMAT = Pattern.compile("(?<index>\\S+)(?<url>.+)");

    /**
     * Parses the given {@code String} of arguments in the context of the {@link AddAvatarCommand}
     * and returns an {@link AddAvatarCommand} object for execution.
     *
     * @throws ParseException if the user input does not conform the expected format
     */
    @Override
    public AddAvatarCommand parse(String args) throws ParseException {
        requireNonNull(args);

        // Defensive programming here to use trim again.
        final Matcher matcher = COMMAND_FORMAT.matcher(args.trim());
        if (!matcher.matches()) {
            throw new ParseException(String.format(MESSAGE_INVALID_COMMAND_FORMAT, AddAvatarCommand.MESSAGE_USAGE));
        }

        try {
            Index index = ParserUtil.parseIndex(matcher.group("index").trim());
            Avatar avatar = new Avatar(matcher.group("url").trim());
            return new AddAvatarCommand(index, avatar);
        } catch (IllegalValueException ive) {
            throw new ParseException(String.format(MESSAGE_INVALID_COMMAND_FORMAT, ive.getMessage()));
        }
    }
}
```
###### \java\seedu\address\logic\parser\person\AddCommandParser.java
``` java
/**
 * Parses input arguments and creates a new AddCommand object
 */
public class AddCommandParser implements Parser<AddCommand> {
    /**
     * Parses the given {@code String} of arguments in the context of the AddCommand
     * and returns an AddCommand object for execution.
     * @throws ParseException if the user input does not conform the expected format
     */
    public AddCommand parse(String args) throws ParseException {
        Set<Prefix> prefixes = PropertyManager.getAllPrefixes();
        prefixes.add(PREFIX_TAG);
        ArgumentMultimap argMultimap = ArgumentTokenizer.tokenize(args, prefixes);

        // TODO: Keep this checking for now. These pre-loaded properties are compulsory.
        if (!ParserUtil.arePrefixesPresent(argMultimap, PREFIX_NAME, PREFIX_ADDRESS, PREFIX_PHONE, PREFIX_EMAIL)) {
            throw new ParseException(String.format(MESSAGE_INVALID_COMMAND_FORMAT, AddCommand.MESSAGE_USAGE));
        }

        try {
            Set<Property> propertyList = ParserUtil.parseProperties(argMultimap.getAllValues());
            Set<Tag> tagList = ParserUtil.parseTags(argMultimap.getAllValues(PREFIX_TAG));
            return new AddCommand(new Person(propertyList, tagList));
        } catch (IllegalValueException | PropertyNotFoundException | DuplicatePropertyException e) {
            throw new ParseException(e.getMessage(), e);
        }
    }
}
```
###### \java\seedu\address\logic\parser\util\ArgumentMultimap.java
``` java
    /**
     * Returns the mapping of {@code Prefix} and their corresponding last values for all {@code prefix}es (only if
     * there is a value present). <b>Notice</b>: the return {@code HashMap} does not include preamble and tags.
     */
    public HashMap<Prefix, String> getAllValues() {
        HashMap<Prefix, String> values = new HashMap<>();

        // Need to manually remove preamble from here. We are creating a new copy of all prefixes, so the actual
        // instance variable will not be affected.
        Set<Prefix> prefixes = new HashSet<>(internalMap.keySet());
        prefixes.remove(new Prefix(""));
        prefixes.remove(PREFIX_TAG);

        for (Prefix prefix: prefixes) {
            getValue(prefix).ifPresent(s -> values.put(prefix, s));
        }

        return values;
    }
```
###### \java\seedu\address\logic\parser\util\CliSyntax.java
``` java
    /* Prefix definitions for adding a new customize property. */
    public static final Prefix PREFIX_SHORT_NAME = new Prefix("s/");
    public static final Prefix PREFIX_FULL_NAME = new Prefix("f/");
    public static final Prefix PREFIX_MESSAGE = new Prefix("m/");
    public static final Prefix PREFIX_REGEX = new Prefix("r/");
```
###### \java\seedu\address\logic\parser\util\NaturalLanguageUtil.java
``` java
/**
 * Utilizes the Natty library to parse datetime representation in human natural language.
 */
public class NaturalLanguageUtil {
    private static Parser nattyParser = new Parser();

    /**
     * Parses a given string representation in human natural language of datetime.
     */
    public static Optional<Date> parseSingleDateTime(String value)
            throws IllegalValueException, PropertyNotFoundException {
        List<DateGroup> groups = nattyParser.parse(value);

        if (groups.isEmpty() || groups.get(0).getDates().isEmpty()) {
            return Optional.empty();
        } else {
            return Optional.of(groups.get(0).getDates().get(0));
        }
    }
}
```
###### \java\seedu\address\logic\parser\util\ParserUtil.java
``` java
    /**
     * Parses all properties in the given {@code HashMap}.
     *
     * @return a set containing all properties parsed.
     */
    public static Set<Property> parseProperties(HashMap<Prefix, String> values)
            throws IllegalValueException, PropertyNotFoundException {
        requireNonNull(values);
        Set<Property> properties = new HashSet<>();

        for (Map.Entry<Prefix, String> entry: values.entrySet()) {
            properties.add(new Property(entry.getKey().getPrefixValue(), entry.getValue()));
        }

        return properties;
    }
```
###### \java\seedu\address\model\Model.java
``` java
    /** Adds a new customize property */
    void addProperty(String shortName, String fullName, String message, String regex)
            throws DuplicatePropertyException, PatternSyntaxException;
```
###### \java\seedu\address\model\Model.java
``` java
    /** Changes the color of an existing tag (through TagColorManager) */
    void setTagColor(Tag tag, String color);

```
###### \java\seedu\address\model\ModelManager.java
``` java
    //=========== Model support for property component =============================================================

    /**
     * Adds a new customize property to {@code PropertyManager}.
     *
     * @throws DuplicatePropertyException if there already exists a property with the same {@code shortName}.
     * @throws PatternSyntaxException     if the given regular expression contains invalid syntax.
     */
    @Override
    public void addProperty(String shortName, String fullName, String message, String regex)
            throws DuplicatePropertyException, PatternSyntaxException {
        PropertyManager.addNewProperty(shortName, fullName, message, regex);
        indicateAddressBookChanged();
    }
```
###### \java\seedu\address\model\ModelManager.java
``` java
    @Override
    public void setPersonAvatar(ReadOnlyPerson target, Avatar avatar) {
        requireAllNonNull(target, avatar);
        target.setAvatar(avatar);
        indicateAddressBookChanged();
    }
```
###### \java\seedu\address\model\ModelManager.java
``` java
    /**
     * Changes the displayed color of an existing tag (through {@link TagColorManager}).
     */
    public void setTagColor(Tag tag, String color) {
        TagColorManager.setColor(tag, color);
        indicateAddressBookChanged();
        raise(new TagColorChangedEvent(tag, color));
    }
```
###### \java\seedu\address\model\person\Avatar.java
``` java
/**
 * Represents the {@link Avatar} image of each {@link Person}. This is a one-to-one relationship, meaning that each
 * {@link Person} should have at most one {@link Avatar}.<br>
 *
 * Notice {@link Avatar} is not a {@link Property}. This is because it is indeed different from other fields of
 * {@link Person}. It is not shown as a row in the {@link PersonDetailsPanel}. Meanwhile, the input validation is
 * done by separate methods rather than a single regular expression (the complexity is not at the same level).
 */
public class Avatar {
    public static final String INVALID_PATH_MESSAGE = "The provided image path is invalid.";
    public static final String IMAGE_NOT_EXISTS = "The provided image path does not exist.";
    public static final String FILE_NOT_IMAGE = "The provided file exists, but it is not an image.";

    private String path;
    private String uri;

    public Avatar(String path) throws IllegalValueException {
        requireNonNull(path);
        if (!isValidAvatarPath(path)) {
            throw new IllegalValueException(INVALID_PATH_MESSAGE);
        }

        File file = new File(path);
        if (!FileUtil.isFileExists(file)) {
            throw new IllegalValueException(IMAGE_NOT_EXISTS);
        }
        if (!FileUtil.isImage(file)) {
            throw new IllegalValueException(FILE_NOT_IMAGE);
        }

        this.path = path;
        this.uri = file.toURI().toString();
    }

    /**
     * An all-in-one checking for the path of the provided image.
     */
    private boolean isValidAvatarPath(String path) {
        return !FileUtil.hasConsecutiveExtensionSeparators(path)
                && !FileUtil.hasConsecutiveNameSeparators(path)
                && !FileUtil.hasInvalidNames(path)
                && !FileUtil.hasInvalidNameSeparators(path);
    }

    public String getPath() {
        return path;
    }

    public String getUri() {
        return uri;
    }

    @Override
    public boolean equals(Object other) {
        return other == this // short circuit if same object
                || (other instanceof Avatar // instanceof handles nulls
                && this.uri.equals(((Avatar) other).uri));
    }

    @Override
    public int hashCode() {
        return uri.hashCode();
    }

    @Override
    public String toString() {
        return "Avatar from " + uri;
    }
}
```
###### \java\seedu\address\model\person\Person.java
``` java
    @Override
    public ObjectProperty<Avatar> avatarProperty() {
        return avatar;
    }

    @Override
    public Avatar getAvatar() {
        return avatar.get();
    }

    public void setAvatar(Avatar avatar) {
        requireNonNull(avatar);
        this.avatar.set(avatar);
    }

    @Override
    public ObjectProperty<UniquePropertyMap> properties() {
        return properties;
    }

    /**
     * Returns an immutable property set, which throws {@code UnsupportedOperationException}
     * if modification is attempted.
     */
    @Override
    public Set<Property> getProperties() {
        return Collections.unmodifiableSet(properties.get().toSet());
    }

    @Override
    public List<Property> getSortedProperties() {
        return Collections.unmodifiableList(properties().get().toSortedList());
    }


    /**
     * Replaces this person's properties with the properties in the argument tag set.
     */
    public void setProperties(Set<Property> replacement) throws DuplicatePropertyException {
        properties.set(new UniquePropertyMap(replacement));
    }

    private String getProperty(String shortName) throws PropertyNotFoundException {
        return properties.get().getPropertyValue(shortName);
    }

    /**
     * Updates the value of the property if there already exists a property with the same shortName, otherwise
     * adds a new property.
     */
    public void setProperty(Property toSet) {
        properties.get().addOrUpdate(toSet);
    }
```
###### \java\seedu\address\model\property\Address.java
``` java
/**
 * Represents a Person's address in the address book.
 * Guarantees: immutable; is valid as declared in {@link #isValidAddress(String)}
 */
public class Address extends Property {
    private static final String PROPERTY_SHORT_NAME = "a";

    public Address(String value) throws IllegalValueException, PropertyNotFoundException {
        super(PROPERTY_SHORT_NAME, value);
    }

    /**
     * Returns true if a given string is a valid address.
     */
    public static boolean isValidAddress(String test) {
        return test.matches(PropertyManager.getPropertyValidationRegex(PROPERTY_SHORT_NAME));
    }
}
```
###### \java\seedu\address\model\property\DateTime.java
``` java
    // To check whether the raw input is in standard format.
    private static final String INPUT_STANDARD_FORMAT = "^(0[1-9]|[12][0-9]|3[01])(0[1-9]|1[012])[0-9]{4}"
            + "(\\s((0[1-9]|1[0-9]|2[0-3]):([0-5][0-9]))?$)";
    // The formatter corresponding to raw input from user.
    private static final SimpleDateFormat inputFormatter = new SimpleDateFormat("ddMMyyyy HH:mm");
    // The formatter corresponding to the format used in UI and storage.
    private static final SimpleDateFormat outputFormatter = new SimpleDateFormat("dd MMM, yyyy HH:mm", Locale.ENGLISH);

    public DateTime(String value) throws IllegalValueException, PropertyNotFoundException {
        super(PROPERTY_SHORT_NAME, prepareDateTimeValue(value));
    }

    public DateTime(Date value) throws IllegalValueException, PropertyNotFoundException {
        super(PROPERTY_SHORT_NAME, formatDateTime(value));
    }

    /**
     * Returns true if a given string is a valid phone number.
     */
    public static boolean isValidTime(String test) {
        try {
            prepareDateTimeValue(test);
            return true;
        } catch (IllegalValueException | PropertyNotFoundException e) {
            return false;
        }
    }

    /**
     * Prepares the value by checking whether the input can be interpreted by the natural language parser.
     */
    private static String prepareDateTimeValue(String value) throws IllegalValueException, PropertyNotFoundException {
        // Returns the original value directly if it is already in standard format.
        if (value.matches(INPUT_STANDARD_FORMAT)) {
            try {
                return formatDateTime(parseDateTime(value));
            } catch (ParseException e) {
                System.err.println("This should never happen. Format check has been performed.");
            }
        }

        Optional<Date> dateObject = NaturalLanguageUtil.parseSingleDateTime(value);
        if (dateObject.isPresent()) {
            return formatDateTime(dateObject.get());
        } else {
            throw new IllegalValueException(PropertyManager.getPropertyConstraintMessage(PROPERTY_SHORT_NAME));
        }
    }

    public static Date parseDateTime(String date) throws ParseException {
        return inputFormatter.parse(date);
    }

    /**
     * Converts the given {@link Date} object into the format used in UI and storage.
     */
    private static String formatDateTime(Date date) {
        return outputFormatter.format(date);
    }
}
```
###### \java\seedu\address\model\property\Email.java
``` java
/**
 * Represents a Person's email in the address book.
 * Guarantees: immutable; is valid as declared in {@link #isValidEmail(String)}
 */
public class Email extends Property {
    private static final String PROPERTY_SHORT_NAME = "e";

    public Email(String value) throws IllegalValueException, PropertyNotFoundException {
        super(PROPERTY_SHORT_NAME, value);
    }

    /**
     * Returns true if a given string is a valid email address.
     */
    public static boolean isValidEmail(String test) {
        return test.matches(PropertyManager.getPropertyValidationRegex(PROPERTY_SHORT_NAME));
    }
}
```
###### \java\seedu\address\model\property\exceptions\DuplicatePropertyException.java
``` java
/**
 * Signals that the property with the same short name already exists.
 */
public class DuplicatePropertyException extends Exception {
    public DuplicatePropertyException(String message) {
        super(message);
    }
}
```
###### \java\seedu\address\model\property\exceptions\PropertyNotFoundException.java
``` java
/**
 * Signals that the required property has not been defined yet.
 */
public class PropertyNotFoundException extends Exception {
    public PropertyNotFoundException() {
        super("Property not found.");
    }

    public PropertyNotFoundException(String shortName) {
        super(String.format(PROPERTY_NOT_FOUND, shortName));
    }
}
```
###### \java\seedu\address\model\property\Name.java
``` java
/**
 * Represents a Person's name in the address book.
 * Guarantees: immutable; is valid as declared in {@link #isValidName(String)}
 */
public class Name extends Property {
    private static final String PROPERTY_SHORT_NAME = "n";

    public Name(String value) throws IllegalValueException, PropertyNotFoundException {
        super(PROPERTY_SHORT_NAME, value);
    }

    /**
     * Returns true if a given string is a valid person name.
     */
    public static boolean isValidName(String test) {
        return test.matches(PropertyManager.getPropertyValidationRegex(PROPERTY_SHORT_NAME));
    }
}
```
###### \java\seedu\address\model\property\Phone.java
``` java
/**
 * Represents a Person's phone number in the address book.
 * Guarantees: immutable; is valid as declared in {@link #isValidPhone(String)}
 */
public class Phone extends Property {
    private static final String PROPERTY_SHORT_NAME = "p";

    public Phone(String value) throws IllegalValueException, PropertyNotFoundException {
        super(PROPERTY_SHORT_NAME, value);
    }

    /**
     * Returns true if a given string is a valid phone number.
     */
    public static boolean isValidPhone(String test) {
        return test.matches(PropertyManager.getPropertyValidationRegex(PROPERTY_SHORT_NAME));
    }
}
```
###### \java\seedu\address\model\property\Property.java
``` java
/**
 * A generic class that represents a property of a person. All properties of a person (including name, email, phone
 * and address) should inherit from this class.
 */
public class Property {
    /**
     * Why do we only store three fields as instance variables in this class?<br>
     *
     * {@link #shortName} is used as the identifier for the property, {@link #fullName} is used stored because we may
     * need to access it frequently (it will be a bad design decision if we have to perform HashMap access operation
     * whenever we need to get the full name of a property), and {@link #value} must be stored here apparently.
     */
    private final String shortName;
    private final String fullName;
    private String value;

    /**
     * Creates a property via its name in short form and its input value.
     *
     * @param shortName is the short name (identifier) of this property.
     */
    public Property(String shortName, String value) throws IllegalValueException, PropertyNotFoundException {
        if (!PropertyManager.containsShortName(shortName)) {
            throw new PropertyNotFoundException(shortName);
        }

        this.shortName = shortName;

        requireNonNull(value);
        if (!isValid(value)) {
            throw new IllegalValueException(PropertyManager.getPropertyConstraintMessage(shortName));
        }
        this.value = value;
        this.fullName = PropertyManager.getPropertyFullName(shortName);
    }

    /**
     * Returns if a given string is a valid value for this property.
     *
     * Notice: Do NOT call this method for {@link DateTime} property. Use {@code DateTime.isValidTime()} instead.
     */
    public boolean isValid(String test) {
        return test.matches(PropertyManager.getPropertyValidationRegex(shortName));
    }

    public String getShortName() {
        return shortName;
    }

    public String getFullName() {
        return fullName;
    }

    public String getValue() {
        return value;
    }

    public void setValue(String value) {
        this.value = value;
    }

    @Override
    public String toString() {
        return value;
    }

    @Override
    public boolean equals(Object other) {
        if (other == this) {
            // short circuit if same object.
            return true;
        } else if (other instanceof Property) {
            // instanceof handles nulls and type checking.
            Property otherProperty = (Property) (other);
            // key-value pair check
            return this.shortName.equals(otherProperty.getShortName())
                    && this.value.equals(otherProperty.getValue());
        } else {
            return false;
        }
    }

    @Override
    public int hashCode() {
        return value.hashCode();
    }
}
```
###### \java\seedu\address\model\property\PropertyManager.java
``` java
/**
 * Manages the different properties (both pre-loaded ones and customize ones) of all persons stored in the
 * application.
 *
 * Pre-loaded properties include {@code Address}, {@code Email}, {@code Name}, {@code Phone}. These pre-loaded
 * properties are subjected to changes in later versions.
 *
 * Customize properties include all properties except the pre-loaded ones, which are added by the following command:
 * <pre>{@code config --add-property <property_name> ...}</pre>
 *
 * TODO: Should we extend {@link seedu.address.commons.core.ComponentManager} as superclass?
 */
public class PropertyManager {
    // Default constraint setting for all properties.
    public static final String DEFAULT_MESSAGE = "%1$s can take any values, but it should not be blank.";
    public static final String DEFAULT_REGEX = "[^\\s].*";

    private static final String DEFAULT_PREFIX = "%1$s/";

    // Mapping from property short name to its prefixes (for all available properties).
    private static final HashMap<String, Prefix> propertyPrefixes = new HashMap<>();

    // Mapping from property short name to its full name.
    private static final HashMap<String, String> propertyFullNames = new HashMap<>();

    // Mapping from property name to the corresponding constraint message and validation regular expression.
    private static final HashMap<String, String> propertyConstraintMessages = new HashMap<>();
    private static final HashMap<String, String> propertyValidationRegex = new HashMap<>();

    // Records whether has been initialized before.
    private static boolean initialized = false;

    /**
     * Util for initialization of default pre-loaded properties. This method should not be called if there is
     * existing data loaded from local storage file.
     */
    public static void initializePropertyManager() {
        if (!initialized) {
            try {
                // Adds name as a pre-loaded property.
                addNewProperty("n", "Name",
                        "Person names should only contain alphanumeric characters and spaces, "
                                + "and it should not be blank.",
                        "[\\p{Alnum}][\\p{Alnum} ]*");

                // Adds email as a pre-loaded property.
                addNewProperty("e", "Email",
                        "Person emails should be 2 alphanumeric/period strings separated by '@'.",
                        "[\\w\\.]+@[\\w\\.]+");

                // Adds phone number as a pre-loaded property.
                addNewProperty("p", "Phone",
                        "Phone numbers can only contain numbers, and should be at least 3 digits long.",
                        "\\d{3,}");

                // Adds address as a pre-loaded property.
                addNewProperty("a", "Address",
                        String.format(DEFAULT_MESSAGE, "Address"), DEFAULT_REGEX);

                // Adds date/time as a pre-loaded property.
                addNewProperty("dt", "DateTime", "Event date & time should be "
                                + "simple and clear enough for the application to understand.", DEFAULT_REGEX);
            } catch (DuplicatePropertyException dpe) {
                throw new AssertionError("Pre-loaded properties cannot be invalid.", dpe);
            }

            initialized = true;
        }
    }

    /**
     * Adds a new available property with all the required information for setting up a property.
     *
     * TODO: Should we allow duplicates in full names of different properties?
     *
     * @param shortName is the short-form name of this property, usually consists of one or two letters, like a
     *                  (stands for address). It is usually the initial of the property name.
     * @param fullName is the full-form name of this property, should be a legal English word.
     * @param message is the constraint message of this property. It will be displayed when the value of this
     *                property does not pass the validation check.
     * @param regex is the regular expression used to perform input validation.
     *
     * @throws DuplicatePropertyException is thrown when a property with the same{@code shortName} already exists.
     * @throws PatternSyntaxException is thrown if the given regex is {@code regex} is invalid.
     */
    public static void addNewProperty(String shortName, String fullName, String message, String regex)
            throws DuplicatePropertyException, PatternSyntaxException {
        // Checks whether there exists a property with the same name.
        if (propertyPrefixes.containsKey(shortName)) {
            throw new DuplicatePropertyException(String.format(PROPERTY_EXISTS, shortName));
        }
        // Checks whether the regular expression is valid.
        Pattern.compile(regex);

        propertyPrefixes.put(shortName, new Prefix(String.format(DEFAULT_PREFIX, shortName)));
        propertyFullNames.put(shortName, fullName);
        propertyConstraintMessages.put(shortName, message);
        propertyValidationRegex.put(shortName, regex);
    }

    /**
     * Clears all properties stored in the {@link PropertyManager}.
     */
    public static void clearAllProperties() {
        propertyPrefixes.clear();
        propertyFullNames.clear();
        propertyConstraintMessages.clear();
        propertyValidationRegex.clear();
    }

    public static boolean containsShortName(String shortName) {
        return propertyPrefixes.containsKey(shortName);
    }

    public static String getPropertyFullName(String shortName) {
        return propertyFullNames.get(shortName);
    }

    public static String getPropertyConstraintMessage(String shortName) {
        return propertyConstraintMessages.get(shortName);
    }

    public static String getPropertyValidationRegex(String shortName) {
        return propertyValidationRegex.get(shortName);
    }

    public static HashSet<String> getAllShortNames() {
        return new HashSet<>(propertyPrefixes.keySet());
    }

    public static HashSet<Prefix> getAllPrefixes() {
        return new HashSet<>(propertyPrefixes.values());
    }
}
```
###### \java\seedu\address\model\property\UniquePropertyMap.java
``` java
/**
 * A HashMap of properties that enforces no nulls and uniqueness between its elements.
 *
 * Supports minimal set of map (list) operations for the app's features.
 *
 * Notice: Uniqueness is directly supported by internal HashMap, which makes it different from
 * {@link seedu.address.model.person.UniquePersonList} and {@link seedu.address.model.tag.UniqueTagList}.
 *
 * @see Property#equals(Object)
 */
public class UniquePropertyMap implements Iterable<Property> {
    private static final String PROPERTY_NOT_FOUND = "This person does not have such property.";
    private final ObservableMap<String, Property> internalMap = FXCollections.observableHashMap();

    /**
     * Constructs empty PropertyList.
     */
    public UniquePropertyMap() {}

    /**
     * Creates a UniquePropertyMap using given properties.
     * Enforces no nulls.
     */
    public UniquePropertyMap(Set<Property> properties) throws DuplicatePropertyException {
        requireAllNonNull(properties);

        for (Property property: properties) {
            add(property);
        }
    }

    /**
     * Returns all properties (collection of values in all entries) in this map as a Set. This set is mutable
     * but change-insulated against the internal map.
     */
    public Set<Property> toSet() {
        return new HashSet<>(internalMap.values());
    }

    /**
     * Returns all properties (collection of values in all entries) in this map as a sorted list based on the full
     * name of each property. This list is mutable but change-insulated against the internal map.
     */
    public List<Property> toSortedList() {
        List<Property> list = new ArrayList<>(internalMap.values());
        list.sort(Comparator.comparing(Property::getFullName));
        return list;
    }

    /**
     * Replaces all the properties in this map with those in the argument property map.
     */
    public void setProperties(Set<Property> properties) throws DuplicatePropertyException {
        requireAllNonNull(properties);
        internalMap.clear();

        for (Property property: properties) {
            add(property);
        }
    }

    /**
     * Merges all properties from the argument list into this list. If a property with the same shortName already
     * exists in the list, it will not be merged in.
     */
    public void mergeFrom(UniquePropertyMap from) {
        for (Property property: from) {
            if (!containsProperty(property)) {
                internalMap.put(property.getShortName(), property);
            }
        }
    }

    /**
     * Returns true if there exists a property with the given shortName in the list.
     */
    public boolean containsProperty(String shortName) {
        requireNonNull(shortName);
        return internalMap.containsKey(shortName);
    }

    /**
     * Returns true if the list containsProperty an equivalent Property (with the same shortName)
     * as the given argument.
     */
    public boolean containsProperty(Property toCheck) {
        requireNonNull(toCheck);
        return containsProperty(toCheck.getShortName());
    }

    public String getPropertyValue(String shortName) throws PropertyNotFoundException {
        if (!containsProperty(shortName)) {
            throw new PropertyNotFoundException();
        }
        return internalMap.get(shortName).getValue();
    }

    /**
     * Adds a property to the map.
     *
     * @throws DuplicatePropertyException if the given property already exists in this list (or there exists a
     * property that is equal to the one in the argument). Since we are using {@link java.util.HashMap}, another
     * method must be used when we want to update the value of an existing property.
     */
    public void add(Property toAdd) throws DuplicatePropertyException {
        requireNonNull(toAdd);
        String shortName = toAdd.getShortName();

        if (containsProperty(shortName)) {
            throw new DuplicatePropertyException(String.format(PROPERTY_EXISTS, shortName));
        }
        internalMap.put(shortName, toAdd);
    }

    /**
     * Updates the value of an existing property in the map.
     *
     * @throws PropertyNotFoundException if there is no property with the same shortName in this map previously.
     */
    public void update(Property toUpdate) throws PropertyNotFoundException {
        requireNonNull(toUpdate);
        String shortName = toUpdate.getShortName();

        if (!containsProperty(shortName)) {
            throw new PropertyNotFoundException();
        }
        internalMap.put(shortName, toUpdate);
    }

    /**
     * Updates the value of the property if there already exists a property with the same shortName, otherwise
     * adds a new property.
     */
    public void addOrUpdate(Property toSet) {
        requireNonNull(toSet);
        String shortName = toSet.getShortName();

        internalMap.put(shortName, toSet);
    }

    @Override
    public Iterator<Property> iterator() {
        return toSet().iterator();
    }

    /**
     * Returns the backing list as an unmodifiable {@code ObservableList}.
     */
    public ObservableMap<String, Property> asObservableList() {
        return FXCollections.unmodifiableObservableMap(internalMap);
    }

    @Override
    public boolean equals(Object other) {
        return other == this // short circuit if same object
                || (other instanceof UniquePropertyMap // instanceof handles nulls
                && this.internalMap.equals(((UniquePropertyMap) other).internalMap));
    }

    /**
     * Utilizes {@link #equals(Object)} because {@link java.util.HashMap} does not enforce
     * ordering anyway.
     */
    public boolean equalsOrderInsensitive(UniquePropertyMap other) {
        return equals(other);
    }

    /**
     * Returns the size of this map.
     */
    public int size() {
        return internalMap.size();
    }

    @Override
    public int hashCode() {
        return internalMap.hashCode();
    }
}
```
###### \java\seedu\address\model\tag\TagColorManager.java
``` java
/**
 * Manages the displayed color of all tags.
 *
 * TODO: Should we extract color out to be a {@code Color} class?
 */
public class TagColorManager {
    /**
     * Stores the colors for all existing tags here so that the same tag always has the same color. Notice this
     * {@code HashMap} has to be declared as a class variable. See the {@code equal} method in {@link Tag} class.
     */
    private static HashMap<Tag, String> internalMap = new HashMap<>();

    // Random number generator (non-secure purpose)
    private static final Random randomGenerator = new Random();

    private static final String TAG_NOT_FOUND = "The provided tag does not exist.";

    /**
     * The upper (exclusive) bound should be equal to {@code Math.pow(16, 6)}. The lower (inclusive) bound should be
     * equal to {@code Math.pow(16, 5)}. Thus, the interval is {@code Math.pow(16, 6) - Math.pow(16, 5)}.
     */
    private static final int RGB_INTERVAL = 15728640;
    private static final int RGB_LOWER_BOUND = 1048576;

    public static String getColor(Tag tag) throws TagNotFoundException {
        if (!internalMap.containsKey(tag)) {
            throw new TagNotFoundException(TAG_NOT_FOUND);
        }

        return internalMap.get(tag);
    }

    public static boolean contains(Tag tag) {
        return internalMap.containsKey(tag);
    }

    /**
     * Changes the color of a specific {@link Tag}.
     *
     * @param tag is the tag whose displayed color will be changed.
     * @param color is the RGB value of its new color.
     */
    public static void setColor(Tag tag, String color) {
        internalMap.put(tag, color);
    }

    /**
     * Randomly assign a color to the given {@code tag}. Notice the selection of random color is not cryptographically
     * secured.
     */
    public static void setColor(Tag tag) {
        int randomColorCode = randomGenerator.nextInt(RGB_INTERVAL) + RGB_LOWER_BOUND;
        setColor(tag, "#" + Integer.toHexString(randomColorCode));
    }
}
```
###### \java\seedu\address\storage\elements\XmlAdaptedPerson.java
``` java
/**
 * JAXB-friendly version of the Person.
 */
public class XmlAdaptedPerson {
    private static final Logger logger = LogsCenter.getLogger(XmlAdaptedPerson.class);
    private static final String IMAGE_NOT_FOUND = "One avatar has been deleted or moved.\n%1$s";

    @XmlElement
    private String avatar;

    @XmlElement
    private List<XmlAdaptedProperty> properties = new ArrayList<>();

    @XmlElement
    private List<XmlAdaptedTag> tagged = new ArrayList<>();

    /**
     * Constructs an XmlAdaptedPerson.
     * This is the no-arg constructor that is required by JAXB.
     */
    public XmlAdaptedPerson() {}

```
###### \java\seedu\address\storage\elements\XmlAdaptedPerson.java
``` java
    /**
     * Converts a given Person into this class for JAXB use.
     *
     * @param source future changes to this will not affect the created XmlAdaptedPerson
     */
    public XmlAdaptedPerson(ReadOnlyPerson source) {
        if (source.getAvatar() != null) {
            avatar = source.getAvatar().getPath();
        }

        properties = new ArrayList<>();
        for (Property property: source.getProperties()) {
            properties.add(new XmlAdaptedProperty(property));
        }

        tagged = new ArrayList<>();
        for (Tag tag : source.getTags()) {
            tagged.add(new XmlAdaptedTag(tag));
        }
    }

    /**
     * Converts this jaxb-friendly adapted person object into the model's Person object.
     *
     * @throws IllegalValueException if there were any data constraints violated in the adapted person
     */
    public Person toModelType() throws IllegalValueException, PropertyNotFoundException, DuplicatePropertyException {
        final List<Property> personProperties = new ArrayList<>();
        for (XmlAdaptedProperty property: properties) {
            personProperties.add(property.toModelType());
        }

        final List<Tag> personTags = new ArrayList<>();
        for (XmlAdaptedTag tag : tagged) {
            personTags.add(tag.toModelType());
        }

        final Set<Property> properties = new HashSet<>(personProperties);
        final Set<Tag> tags = new HashSet<>(personTags);
        final Person person = new Person(properties, tags);

        if (avatar != null) {
            try {
                person.setAvatar(new Avatar(avatar));
            } catch (IllegalValueException ive) {
                logger.warning(String.format(IMAGE_NOT_FOUND, ive.getMessage()));
            }
        }

        return person;
    }
}
```
###### \java\seedu\address\storage\elements\XmlAdaptedProperty.java
``` java
/**
 * JAXB-friendly adapted version of the {@link Property}, stored within each person.
 */
public class XmlAdaptedProperty {
    @XmlAttribute
    private String shortName;
    @XmlValue
    private String value;

    /**
     * Constructs an XmlAdaptedProperty.
     * This is the no-arg constructor that is required by JAXB.
     */
    public XmlAdaptedProperty() {}

    /**
     * Converts a given Property into this class for JAXB use.
     *
     * @param source future changes to this will not affect the created
     */
    public XmlAdaptedProperty(Property source) {
        this.shortName = source.getShortName();
        this.value = source.getValue();
    }

    /**
     * Converts this jaxb-friendly adapted property object into the model's Property object.
     *
     * @return a Property object used in model.
     * @throws IllegalValueException if there were any data constraints violated in the adapted property.
     * @throws PropertyNotFoundException the same as above.
     */
    public Property toModelType() throws IllegalValueException, PropertyNotFoundException {
        return new Property(shortName, value);
    }
}
```
###### \java\seedu\address\storage\elements\XmlAdaptedPropertyInfo.java
``` java
/**
 * JAXB-friendly adapted version of the {@link Property}, stores the general information of each property.
 */
public class XmlAdaptedPropertyInfo {
    @XmlElement
    private String shortName;
    @XmlElement
    private String fullName;
    @XmlElement
    private String message;
    @XmlElement
    private String regex;

    /**
     * Constructs an XmlAdaptedTag.
     * This is the no-arg constructor that is required by JAXB.
     */
    public XmlAdaptedPropertyInfo() {}

    public XmlAdaptedPropertyInfo(String shortName, String fullName, String message, String regex) {
        this.shortName = shortName;
        this.fullName = fullName;
        this.message = message;
        this.regex = regex;
    }

    public void toModelType() throws DuplicatePropertyException {
        PropertyManager.addNewProperty(shortName, fullName, message, regex);
    }
}
```
###### \java\seedu\address\storage\elements\XmlAdaptedPropertyManager.java
``` java
/**
 * JAXB-friendly adapted version of the {@link PropertyManager}.
 */
public class XmlAdaptedPropertyManager {
    @XmlElement
    private List<XmlAdaptedPropertyInfo> property;

    public XmlAdaptedPropertyManager() {
        property = new ArrayList<>();
        for (String shortName: PropertyManager.getAllShortNames()) {
            XmlAdaptedPropertyInfo info = new XmlAdaptedPropertyInfo(shortName,
                    PropertyManager.getPropertyFullName(shortName),
                    PropertyManager.getPropertyConstraintMessage(shortName),
                    PropertyManager.getPropertyValidationRegex(shortName));
            property.add(info);
        }
    }

    /**
     * Initialize all properties by adding them to {@link PropertyManager}.
     */
    public void initializeProperties() {
        try {
            for (XmlAdaptedPropertyInfo info: property) {
                info.toModelType();
            }
        } catch (DuplicatePropertyException dpe) {
            // TODO: better error handling
            dpe.printStackTrace();
        }
    }
}
```
###### \java\seedu\address\storage\elements\XmlSerializableAddressBook.java
``` java
    /**
     * Initialize the {@link PropertyManager} by clearing all existing properties and load information about new
     * properties from the storage file.
     */
    public void initializePropertyManager() {
        PropertyManager.clearAllProperties();
        properties.initializeProperties();
    }
```
###### \java\seedu\address\ui\MainWindow.java
``` java
    /**
     * Take note of the following two methods, which overload each other. The one without parameter is used as the
     * callback when the user clicks on the sidebar button; the other one is used as the subscriber when the user
     * enters some command(s) that raise(s) the corresponding event(s).
     */
    @FXML
    private void handleSwitchToContacts() {
        dataDetailsPanelPlaceholder.getChildren().clear();
        dataListPanelPlaceholder.getChildren().clear();
        dataListPanelPlaceholder.getChildren().add(personListPanel.getRoot());
    }

    @Subscribe
    public void handleSwitchToContacts(SwitchToContactsListEvent event) {
        dataDetailsPanelPlaceholder.getChildren().clear();
        dataListPanelPlaceholder.getChildren().clear();
        dataListPanelPlaceholder.getChildren().add(personListPanel.getRoot());
    }

    /**
     * Similar to methods for contacts except with EventCalendar.
     */
    @FXML
    private void handleSwitchToEvents() {
        dataDetailsPanelPlaceholder.getChildren().clear();
        dataListPanelPlaceholder.getChildren().clear();
        dataListPanelPlaceholder.getChildren().add(eventListPanel.getRoot());
        dataDetailsPanelPlaceholder.getChildren().add(new EventCalendar().getRoot());
    }

    @Subscribe
    public void handleSwitchToEvents(SwitchToEventsListEvent event) {
        dataListPanelPlaceholder.getChildren().clear();
        dataListPanelPlaceholder.getChildren().add(eventListPanel.getRoot());
        dataDetailsPanelPlaceholder.getChildren().clear();
        dataDetailsPanelPlaceholder.getChildren().add(new EventCalendar().getRoot());
    }

    @Subscribe
    private void handlePersonPanelSelectionChangedEvent(PersonPanelSelectionChangedEvent event) {
        logger.info(LogsCenter.getEventHandlingLogMessage(event));
        ReadOnlyPerson person = event.getNewSelection().person;

        dataDetailsPanelPlaceholder.getChildren().clear();
        dataDetailsPanelPlaceholder.getChildren().add(new PersonDetailsPanel(person).getRoot());
    }
```
###### \java\seedu\address\ui\person\PersonCard.java
``` java
    /**
     * Initializes all the tags of a person displayed in different random colors.
     */
    private void initTags() {
        person.getTags().forEach(tag -> {
            String tagName = tag.tagName;
            Label newTagLabel = new Label(tagName);
            try {
                newTagLabel.setStyle(String.format(TAG_COLOR_CSS, TagColorManager.getColor(tag)));
            } catch (TagNotFoundException e) {
                System.err.println("An existing must have a color.");
            }
            tags.getChildren().add(newTagLabel);
        });
    }

    @Subscribe
    public void handleTagColorChange(TagColorChangedEvent event) {
        // TODO: improve efficiency here. Update rather than re-create all labels.
        tags.getChildren().clear();
        initTags();
    }
```
###### \java\seedu\address\ui\person\PersonCard.java
``` java

    @Override
    public boolean equals(Object other) {
        // short circuit if same object
        if (other == this) {
            return true;
        }

        // instanceof handles nulls
        if (!(other instanceof PersonCard)) {
            return false;
        }

        // state check
        PersonCard card = (PersonCard) other;
        return id.getText().equals(card.id.getText())
                && person.equals(card.person);
    }
}
```
###### \java\seedu\address\ui\person\PersonDetailsPanel.java
``` java
/**
 * The panel on the right side of {@link PersonListPanel}. Used to show the details (including photo and all
 * properties) of a specific person (selected on the {@link PersonListPanel}).
 */
public class PersonDetailsPanel extends UiPart<Region> {
    private static final String FXML = "person/PersonDetailsPanel.fxml";

    private ReadOnlyPerson person;

    @FXML
    private Label name;
    @FXML
    private ImageView avatar;
    @FXML
    private ListView<Label> propertyListKeys;
    @FXML
    private ListView<Label> propertyListValues;

    public PersonDetailsPanel(ReadOnlyPerson person) {
        super(FXML);
        this.person = person;
        name.textProperty().bind(Bindings.convert(person.nameProperty()));
        person.avatarProperty().addListener((observable, oldValue, newValue) -> setAvatar());
        setAvatar();
        person.properties().addListener((observable, oldValue, newValue) -> bindProperties());
        bindProperties();
    }

    /**
     * Binds all properties of this person to a {@link ListView} of key-value pairs.
     */
    private void bindProperties() {
        List<Label> keys = new ArrayList<>();
        List<Label> values = new ArrayList<>();

        person.getSortedProperties().forEach(property -> {
            Label newPropertyKey = new PropertyLabel(property.getFullName() + ":", "details-property-key");
            Label newPropertyValue = new PropertyLabel(property.getValue(), "details-property-value");

            keys.add(newPropertyKey);
            values.add(newPropertyValue);
        });

        propertyListKeys.setItems(FXCollections.observableList(keys));
        propertyListValues.setItems(FXCollections.observableList(values));
    }

    /**
     * Displays the avatar of the person if the {@code avatar} has been set before.
     */
    private void setAvatar() {
        if (person.getAvatar() != null) {
            Platform.runLater(() -> avatar.setImage(new Image(person.getAvatar().getUri(), 200, 200, false, true)));
        }
    }
}
```
###### \java\seedu\address\ui\PropertyLabel.java
``` java
/**
 * A customize JavaFX {@link Label} class used to display the key-value pairs of all properties.
 */
public class PropertyLabel extends Label {
    public PropertyLabel(String text, String style) {
        super(text);
        this.getStyleClass().add(style);
    }
}
```
###### \resources\css\Extensions.css
``` css
.sidebar-button {
    -fx-background-insets: 0,1,2;
    -fx-background-radius: 3,2,1;
    -fx-text-fill: black;
    -fx-font-size: 14px;
}

.details-name-huge-label {
    -fx-font-family: "Segoe UI Semibold";
    -fx-font-size: 40px;
    -fx-text-fill: #fffde7;
}

.details-property-key {
    -fx-font-family: "Segoe UI Light";
    -fx-font-size: 25px;
    -fx-text-fill: white;
    -fx-text-alignment: left;
}

.details-property-value {
    -fx-font-size: 25px;
    -fx-text-fill: white;
    -fx-text-alignment: left;
}
```
###### \resources\view\MainWindow.fxml
``` fxml
  <SplitPane id="splitPane" fx:id="splitPane" dividerPositions="0.4, 0.5" minWidth="600.0" prefWidth="1000.0" VBox.vgrow="ALWAYS">
      <VBox fx:id="sideButtonBar" alignment="CENTER" maxWidth="80.0" minWidth="80.0" prefWidth="80.0">
         <padding>
            <Insets bottom="10.0" top="10.0" />
         </padding>
         <children>
            <ImageView fx:id="switchToContactsButton" fitHeight="50.0" fitWidth="50.0" onMouseClicked="#handleSwitchToContacts" pickOnBounds="true" styleClass="sidebar-button">
               <VBox.margin>
                  <Insets bottom="50.0" top="50.0" />
               </VBox.margin>
               <image>
                  <Image url="@../images/contacts.png" />
               </image>
            </ImageView>
            <ImageView fx:id="switchToEventsButton" fitHeight="50.0" fitWidth="50.0" layoutX="20.0" layoutY="20.0" onMouseClicked="#handleSwitchToEvents" pickOnBounds="true" styleClass="sidebar-button">
               <image>
                  <Image url="@../images/events.png" />
               </image>
               <VBox.margin>
                  <Insets bottom="50.0" top="50.0" />
               </VBox.margin>
            </ImageView>
         </children>
      </VBox>
    <VBox fx:id="dataList" minWidth="340" prefWidth="340.0" SplitPane.resizableWithParent="false">
      <padding>
        <Insets bottom="10" left="10" right="10" top="10" />
      </padding>
      <StackPane fx:id="dataListPanelPlaceholder" VBox.vgrow="ALWAYS" />
    </VBox>

    <StackPane fx:id="dataDetailsPanelPlaceholder">
      <padding>
        <Insets bottom="10" left="10" right="10" top="10" />
      </padding>
    </StackPane>
  </SplitPane>
```
###### \resources\view\person\PersonDetailsPanel.fxml
``` fxml


<?import javafx.geometry.Insets?>
<?import javafx.scene.control.Label?>
<?import javafx.scene.control.ListView?>
<?import javafx.scene.image.Image?>
<?import javafx.scene.image.ImageView?>
<?import javafx.scene.layout.HBox?>
<?import javafx.scene.layout.VBox?>
<?import javafx.scene.text.Font?>
<VBox prefHeight="600.0" prefWidth="580.0" xmlns="http://javafx.com/javafx/8.0.141" xmlns:fx="http://javafx.com/fxml/1">

   <children>
      <HBox prefHeight="200.0">
         <children>
            <ImageView fx:id="avatar" fitHeight="200.0" fitWidth="200.0" pickOnBounds="true" preserveRatio="true">
               <image>
                  <Image url="@../../images/default_person_photo.png" />
               </image>
            </ImageView>
            <Label fx:id="name" styleClass="details-name-huge-label" text="\\\$name" wrapText="true">
               <font>
                  <Font size="45.0" />
               </font>
               <HBox.margin>
                  <Insets left="30.0" top="50.0" />
               </HBox.margin>
            </Label>
         </children>
      </HBox>
      <HBox>
         <children>
            <ListView fx:id="propertyListKeys" />
            <ListView fx:id="propertyListValues" prefWidth="500.0" />
         </children>
         <VBox.margin>
            <Insets top="30.0" />
         </VBox.margin>
      </HBox>
   </children>
</VBox>
```
